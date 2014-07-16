/*
 * =====================================================================================
 *
 *       Filename:  strings.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014/07/15 10时04分57秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>

#include "strings.h"
#include "cmalloc.h"

dst dsnewlen(cdst init,size_t initlen){
    struct dshdr *sh;
    if(init){
        sh = cmalloc(sizeof(*sh)+initlen+1);
    }else{
        sh = ccalloc(sizeof(*sh)+initlen+1);
    }

    if(sh == NULL) return NULL;
    sh->len = initlen;
    sh->free =0;
    if(initlen && init)
        memcpy(sh->buf,init,initlen);
    sh->buf[initlen] = '\0';
    return (dst)sh->buf;
}
dst dscatds(dst s,cdst t){
    return NULL;
}

dst dsempty(void){
    return dsnewlen("",0);
}

dst dsnew(const char *init){
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return dsnewlen(init,initlen);
}

dst dsdup(const dst s){
    return dsnewlen(s,dslen(s));
}

void dsfree(dst s){
    if(s == NULL) return;
    struct dshdr *sh = (void*)(s-(sizeof(struct dshdr)));
    cfree(sh);
}

/* 
 * Append the specified binary-safe string pointed by t of len bytes to 
 * the end of the specified dst string 's'.
 * */
dst dscatlen(dst s, const void *t, size_t len){
    struct dshdr *sh;
    size_t curlen = dslen(s);

    s = dsMakeRoomFor(s,len);
    if(s == NULL) return NULL;
    sh = (void*)(s-(sizeof(*sh)));
    memcpy(s+curlen,t,len);
    sh->len = curlen+len;
    sh->free = sh->free-len;
    s[curlen+len] = '\0';
    return s;
}

dst dscat(dst s,cdst t){
    return dscatlen(s,t,strlen(t));
}

dst dsMakeRoomFor(dst s, size_t addlen){
    struct dshdr *sh, *newsh;
    size_t free = dsavail(s);
    size_t len, newlen;
    if(free >= addlen) return s;
    len = dslen(s);
    sh = (void*)(s-(sizeof(struct dshdr)));
    newlen = (len+addlen);
    if(newlen < ds_MAX_PREALLOC){
        newlen *= 2;
    }else{
        newlen += ds_MAX_PREALLOC;
    }
    newsh = crealloc(sh,sizeof(struct dshdr)+newlen+1);
    if(newsh == NULL) return NULL;
    newsh->free = newlen - len;
    return newsh->buf;
}

