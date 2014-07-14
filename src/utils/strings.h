/* SDSLib, A C dynamic strings library
 *
 * Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _UTILS_STRINGS
#define _UTILS_STRINGS
#define SDS_MAX_PREALLOC (1024*1024)
#include <sys/types.h>
#include <stdarg.h>

typedef const char *sdt;
typedef char *sds;

struct sdshdr{
    int len;
    int free;
    char buf[];
};

static inline size_t sdslen(sdt s){
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

static inline size_t sdsavail(sdt s){
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}

sds sdtnewlen(sdt init,size_t initlen);
sds sdsnew(sdt init);
sds sdsempty(void);
size_t sdslen(sdt s);
sds sdsup(sdt s);
void sdsfree(sds s);
size_t sdsavail(sdt s);
sds sdsgrowzero(sds s, size_t len);
sds sdscatlen(sds s, const void *t, size_t len);
sds sdscat(sds s, sdt t);
sds sdscatsds(sds s, sdt t);
sds sdscpylen(sds s, sdt t,size_t len);
sds sdscpy(sds s, sdt t);
#ifdef __GNUC__
sds sdscatprintf(sds s,sdt fmt, ...)
    __attribute__((format(printf,2,3)));
#else
sds sdscatprintf(sds s,sdt fmt, ...);
#endif

sds sdscatfmt(sds s,sdt fmt, ...);
sds sdstrim(sds s, sdt cset);
void sdsrange(sds s, int start, int end);
void sdsupdatelen(sds s);
void clear(sds s);
int sdscmp(sdt s1,sdt s2);
sds *sdssplitlen(sdt s,int len,sdt sep,int seplen,int count);
void sdsfreesplitres(sds *tokens,int count);
void sdstolower(sds s);
void sdstoupper(sds s);
sds sdsfromlonglong(long long value);
sds sdscatrepr(sds s, sdt p, size_t len);
sds *sdssplitargs(sdt line,int *argc);
sds sdsmapchars(sds s,sdt from,sdt to,size_t setlen);
sds sdsjoin(char **argv,int argc,char *sep);

sds sdsMakeRoomFor(sds s,size_t addlen);
void sdsIncrLen(sds s, int incr);
sds sdsRemoveFreeSpace(sds s);
size_t sdsAllocSize(sds s);

#endif
