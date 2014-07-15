/*
 * =====================================================================================
 *
 *       Filename:  cmalloc.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014/07/15 14时39分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>

void clibc_free(void *ptr){
    free(ptr);
}

#include <string.h>
#include <pthread.h>
#include "cmalloc.h"

#ifdef HAVE_MALLOC_SIZE
#define PREFIX_SIZE (0)
#else
#if defined(__sun) || defined(__sparc) || defined(__sparc__)
#define PREFIX_SIZE (sizeof(long long))
#else
#define PREFIX_SIZE (sizeof(size_t))
#endif
#endif

/* Explicitly override malloc/free etc when using tcmalloc. */
#if defined(USE_TCMALLOC)
#define malloc(size) tc_malloc(size)
#define calloc(count,size) tc_calloc(count,size)
#define realloc(ptr,size) tc_realloc(ptr,size)
#define free(ptr) tc_free(ptr)
#elif defined(USE_JEMALLOC)
#define malloc(size) je_malloc(size)
#define calloc(count,size) je_calloc(count,size)
#define realloc(ptr,size) je_realloc(ptr,size)
#define free(ptr) je_free(ptr)
#endif

#ifdef HAVE_ATOMIC
#define update_cmalloc_stat_add(__n) __sync_add_and_fetch(&used_memory,(__n))
#define update_cmalloc_stat_sub(__n) __sync_sub_and_fetch(&used_memory,(__n))
#else
#define update_cmalloc_stat_add(__n) do { \
    pthread_mutex_lock(&used_memory_mutex); \
    used_memory += (__n); \
    pthread_mutex_unlock(&used_memory_mutex); \
} while(0)

#define update_cmalloc_stat_sub(__n) do { \
    pthread_mutex_lock(&used_memory_mutex); \
    used_memory -= (__n); \
    pthread_mutex_unlock(&used_memory_mutex); \
} while(0)
#endif


#define update_cmalloc_stat_alloc(__n) do { \
    size_t _n = (__n); \
    if (_n&(sizeof(long)-1)) _n += sizeof(long)-(_n&(sizeof(long)-1)); \
    if (cmalloc_thread_safe) { \
        update_cmalloc_stat_add(_n); \
    } else { \
        used_memory += _n; \
    } \
} while(0)

#define update_cmalloc_stat_free(__n) do { \
    size_t _n = (__n); \
    if (_n&(sizeof(long)-1)) _n += sizeof(long)-(_n&(sizeof(long)-1)); \
    if (cmalloc_thread_safe) { \
        update_cmalloc_stat_sub(_n); \
    } else { \
        used_memory -= _n; \
    } \
} while(0)


static size_t used_memory = 0;
static int cmalloc_thread_safe = 0;
pthread_mutex_t used_memory_mutex = PTHREAD_MUTEX_INITIALIZER;

static void cmalloc_default_oom(size_t size){
    fprintf(stderr, "cmalloc: OOM trying to allocate %zu bytes\n",
            size);
    fflush(stderr);
    abort(); 
}

static void (*cmalloc_oom_handler)(size_t) = cmalloc_default_oom;



void *cmalloc(size_t size){
    void *ptr = malloc(size+PREFIX_SIZE);

    if (!ptr) cmalloc_oom_handler(size);
    printf("test %zu;\n",size);
#ifdef HAVE_MALLOC_SIZE
    update_cmalloc_stat_alloc(cmalloc_size(ptr));
    return ptr;
#else
    *((size_t*)ptr) = size;
    update_cmalloc_stat_alloc(size+PREFIX_SIZE);
    return (char*)ptr+PREFIX_SIZE;
#endif

}

void *ccalloc(size_t size) {
    void *ptr = calloc(1,size+PREFIX_SIZE);

    if (!ptr) cmalloc_oom_handler(size);
#ifdef HAVE_MALLOC_SIZE
    update_cmalloc_stat_alloc(cmalloc_size(ptr));
    return ptr;
#else
    *((size_t*)ptr) = size;
    update_cmalloc_stat_alloc(size+PREFIX_SIZE);
    return (char*)ptr+PREFIX_SIZE;
#endif
}

void *crealoc(void *ptr, size_t size){
#ifndef HAVE_MALLOC_SIZE
    void *realptr;
#endif
    size_t oldsize;
    void *newptr;

    if(ptr == NULL) return cmalloc(size);
#ifdef HAVE_MALLOC_SIZE
    oldsize = cmalloc_size(ptr);
    newptr = realloc(ptr,size);
    if(!newptr) cmalloc_oom_handler(size);
    
    update_cmalloc_stat_free(oldsize);
    update_cmalloc_stat_alloc(cmalloc_size(newptr));
    return newptr;
#else
    realptr = (char*)ptr-PREFIX_SIZE;
    oldsize = *((size_t*)realptr);
    newptr  = realoc(realptr,size+PREFIX_SIZE);
    if(!newptr) cmalloc_oom_handler(size);
    
    *((size_t*)newptr) = size;
    update_cmalloc_stat_free(oldsize);
    update_cmalloc_stat_alloc(size);
    return (char*)newptr+PREFIX_SIZE;
#endif
}

#ifndef HAVE_MALLOC_SIZE
size_t cmalloc_size(void *ptr){
    void *realtr = (char*)ptr-PREFIX_SIZE;
    size_t size = *((size_t*)realtr);
    if (size&(sizeof(long)-1)) size += sizeof(long)-(size&(sizeof(long)-1));
    return size+PREFIX_SIZE;
}
#endif

void cfree(void *ptr){
#ifndef HAVE_MALLOC_SIZE
    void *realptr;
    size_t oldsize;
#endif
    if(ptr == NULL) return;
#ifdef HAVE_MALLOC_SIZE
    update_cmalloc_stat_free(cmalloc_size(ptr));
    free(ptr);
#else
    realptr = (char*)ptr-PREFIX_SIZE;
    oldsize = *((size_t*)realptr);
    update_cmalloc_stat_free(oldsize+PREFIX_SIZE);
    free(realptr);
#endif
}

char *cstrdup(sdt s){
    size_t l = strlen(s) + 1;
    char *p = cmalloc(l);
    memcpy(p,s,l);
    return p;
}

size_t cmalloc_used_memory(void) {
    size_t um;

    if (cmalloc_thread_safe) {
#ifdef HAVE_ATOMIC
        um = __sync_add_and_fetch(&used_memory, 0);
#else
        pthread_mutex_lock(&used_memory_mutex);
        um = used_memory;
        pthread_mutex_unlock(&used_memory_mutex);
#endif
    }
    else {
        um = used_memory;
    }

    return um;
}

void cmalloc_enable_thread_safeness(void) {
    cmalloc_thread_safe = 1;
}

void cmalloc_set_oom_handler(void (*oom_handler)(size_t)) {
    cmalloc_oom_handler = oom_handler;
}

/* Get the RSS information in an OS-specific way.
 *
 * WARNING: the function zmalloc_get_rss() is not designed to be fast
 * and may not be called in the busy loops where Redis tries to release
 * memory expiring or swapping out objects.
 *
 * For this kind of "fast RSS reporting" usages use instead the
 * function RedisEstimateRSS() that is a much faster (and less precise)
 * version of the function. */

#if defined(HAVE_PROC_STAT)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

size_t cmalloc_get_rss(void) {
    int page = sysconf(_SC_PAGESIZE);
    size_t rss;
    char buf[4096];
    char filename[256];
    int fd, count;
    char *p, *x;

    snprintf(filename,256,"/proc/%d/stat",getpid());
    if ((fd = open(filename,O_RDONLY)) == -1) return 0;
    if (read(fd,buf,4096) <= 0) {
        close(fd);
        return 0;
    }
    close(fd);

    p = buf;
    count = 23; /* RSS is the 24th field in /proc/<pid>/stat */
    while(p && count--) {
        p = strchr(p,' ');
        if (p) p++;
    }
    if (!p) return 0;
    x = strchr(p,' ');
    if (!x) return 0;
    *x = '\0';

    rss = strtoll(p,NULL,10);
    rss *= page;
    return rss;
}
#elif defined(HAVE_TASKINFO)
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/task.h>
#include <mach/mach_init.h>

size_t cmalloc_get_rss(void) {
    task_t task = MACH_PORT_NULL;
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    if (task_for_pid(current_task(), getpid(), &task) != KERN_SUCCESS)
        return 0;
    task_info(task, TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count);

    return t_info.resident_size;
}
#else
size_t cmalloc_get_rss(void) {
    /* If we can't get the RSS in an OS-specific way for this system just
     * return the memory usage we estimated in zmalloc()..
     *
     * Fragmentation will appear to be always 1 (no fragmentation)
     * of course... */
    return cmalloc_used_memory();
}
#endif

/* Fragmentation = RSS / allocated-bytes */
float cmalloc_get_fragmentation_ratio(size_t rss) {
    return (float)rss/cmalloc_used_memory();
}

#if defined(HAVE_PROC_SMAPS)
size_t cmalloc_get_private_dirty(void) {
    char line[1024];
    size_t pd = 0;
    FILE *fp = fopen("/proc/self/smaps","r");

    if (!fp) return 0;
    while(fgets(line,sizeof(line),fp) != NULL) {
        if (strncmp(line,"Private_Dirty:",14) == 0) {
            char *p = strchr(line,'k');
            if (p) {
                *p = '\0';
                pd += strtol(line+14,NULL,10) * 1024;
            }
        }
    }
    fclose(fp);
    return pd;
}
#else
size_t cmalloc_get_private_dirty(void) {
    return 0;
}
#endif
