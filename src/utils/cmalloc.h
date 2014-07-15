/*
 * =====================================================================================
 *
 *       Filename:  cmalloc.h
 *
 *    Description:  merror admin
 *
 *        Version:  1.0
 *        Created:  2014/07/15 10时08分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _UTILS_CMALLOC
#define _UTILS_CMALLOC
#include "strings.h"

#define __xstr(s) __str(s)
#define __str(s) #s

#if defined(USE_TCMALLOC)
#define UMALLOC_LIB("tcmalloc-" __xstr(TC_VERSION_MAJOR)"."__xstr(TC_VERSION_MINOR))
#include <google/tcmalloc.h>
#if (TC_VERSION_MAJOR == 1 && TC_VERSION_MAJOR >= 6) || (TC_VERSION_MAJOR > 1)
#define HAVE_MALLOC_SIZE 1
#define cmalloc_size(p) tc_malloc_size(p)
#else
#error "Newer version of tcmalloc required"
#endif

#elif defined(USE_JEMALLOC)
#define ZMALLOC_LIB("jemalloc-" __xstr(JEMALLOC_VERSION_MAJOR)"."__xstr(JEMALLOC_VERSION_MINOR)"."__xstr(JEMALLOC_VERSION_BUGFIX))
#include <jemalloc/jemalloc.h>
#if(JEMALLOC_VERSION_MAJOR == 2 && JEMALLOC_VERSION_MINOR >= 1) || (JEMALLOC_VERSION_MAJOR > 2)
#define HAVE_MALLOC_SIZE 1
#define cmalloc_size(p) je_malloc_usable_size()
#else
#error "Newer version of jemalloc required"
#endif

#elif defined(__APPLE__)
#include <malloc/malloc.h>
#define HAVE_MALLOC_SIZE 1
#define cmalloc_size(p) malloc_size(p)
#endif

#ifndef _UTILS_CMALLOC_LIB
#define _UTILS_CMALLOC_LIB "libc"
#endif

void *cmalloc(size_t size);
void *ccalloc(size_t size);
void *crealloc(void *ptr, size_t size);
void cfree(void *ptr);
char *cstrdup(sdt s);
size_t cmalloc_used_memory(void);
void cmalloc_enable_thread_safeness(void);
void cmalloc_set_oom_handler(void (*oom_handler)(size_t));
float cmalloc_get_fragmentation_ratio(size_t rss);
size_t cmalloc_get_rss(void);
size_t cmalloc_get_private_dirty(void);
void clibc_free(void *ptr);

#ifndef HAVE_MALLOC_SIZE
size_t cmalloc_size(void *ptr);
#endif



#endif


