/* dsLib, A C dynamic strings library
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
#define ds_MAX_PREALLOC (1024*1024)
#include <sys/types.h>
#include <stdarg.h>

typedef const char *cdst;
typedef char *dst;

struct dshdr{
    int len;
    int free;
    char buf[];
};

static inline size_t dslen(cdst s) {
    struct dshdr *sh = (void*)(s-(sizeof(struct dshdr)));
    return sh->len;
}

static inline size_t dsavail(cdst s) {
    struct dshdr *sh = (void*)(s-(sizeof(struct dshdr)));
    return sh->free;
}


dst dtnewlen(cdst init,size_t initlen);
dst dsnew(cdst init);
dst dsempty(void);
size_t dslen(cdst s);
dst dsup(cdst s);
void dsfree(dst s);
size_t dsavail(cdst s);
dst dsgrowzero(dst s, size_t len);
dst dscatlen(dst s, const void *t, size_t len);
dst dscat(dst s, cdst t);
dst dscatds(dst s, cdst t);
dst dscpylen(dst s, cdst t,size_t len);
dst dscpy(dst s, cdst t);
#ifdef __GNUC__
dst dscatprintf(dst s,cdst fmt, ...)
    __attribute__((format(printf,2,3)));
#else
dst dscatprintf(dst s,cdst fmt, ...);
#endif

dst dscatfmt(dst s,cdst fmt, ...);
dst dstrim(dst s, cdst cset);
void dsrange(dst s, int start, int end);
void dsupdatelen(dst s);
void clear(dst s);
int dscmp(cdst s1,cdst s2);
dst *dssplitlen(cdst s,int len,cdst sep,int seplen,int count);
void dsfreesplitres(dst *tokens,int count);
void dstolower(dst s);
void dstoupper(dst s);
dst dsfromlonglong(long long value);
dst dscatrepr(dst s, cdst p, size_t len);
dst *dssplitargs(cdst line,int *argc);
dst dsmapchars(dst s,cdst from,cdst to,size_t setlen);
dst dsjoin(char **argv,int argc,char *sep);

dst dsMakeRoomFor(dst s,size_t addlen);
void dsIncrLen(dst s, int incr);
dst dsRemoveFreeSpace(dst s);
size_t dsAllocSize(dst s);



#endif
