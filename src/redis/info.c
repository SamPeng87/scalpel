/*
 * =====================================================================================
 *
 *       Filename:  info.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014/07/11 14时20分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "info.h"
#include "../utils/cmalloc.h"
#include "../utils/strings.h"
#include "../utils/config.h"

#define parseKeyAndValue(s,key,value) \
    int len = 0; \
    while(*s){ \
        if(*s == ':'){ \
            *s = '\0'; \
            key = s-len; \
            value = s+1; \
        } \
        len ++; \
        ++s; \
    } \
;

#define INFO_COMMAND "info"

redisReply *getRedisInfoChar(redisContext *context) {
   const char* infoCommand = INFO_COMMAND; 
   return (redisReply*) redisCommand(context,infoCommand);
}

char **getInfoByString(dst ch,int *count){
    int slots =5;
    char **splits = cmalloc(sizeof(char *)*slots);

    int i=0;
    int len=0;
    int start=0;
    do{
        if(*ch == '\n'){
            if(slots < i+2){
                char **newsplits;
                slots *= 2;
                newsplits = crealloc(splits,sizeof(char *)*slots);
                if(isNull(newsplits)) goto cleanup;
                splits = newsplits;
            }
            *ch = '\0';

            if(start == 0 ){
                splits[i] = ch-(len-start);
            }else{
                splits[i] = ch-(len-start-1);
            }
            start = len;
            i++;
        }
        len ++;
        ++ch;
    }while(*ch);
    *count = i;
    return splits;
cleanup :
    {
        cfree(splits);
        *count =0;
        return NULL;
    }
}

void switchValueOfObj(redisInfo *rInfo,char *key,char *value){
    //TODO need impl
    printf("%s\n",key);
}


void parseInfo(redisInfo *rInfo,char **info,int count){
    int j=0;
    for(;j< count;j++){
        char *rik = info[j]; 
        char *key = NULL;
        char *value = NULL;
        parseKeyAndValue(rik,key,value);
        switchValueOfObj(rInfo,key,value);
    }
    
}

redisInfo *getRedisInfoObj(dst ch){
    redisInfo *rInfo = ccalloc(sizeof(redisInfo));
    char** info;
    if(NULL == ch){
        rInfo->err = 1;
        rInfo->errstr = "redis info is null";
        return rInfo;
    }

    if( (strncmp(ch,"ERR",2)) == 0){
        rInfo->err = 2;
        dst s= dsnew("redis info can't support command:");
        rInfo->errstr = dscat(s,INFO_COMMAND);
        return rInfo;
    }


    int len=0;
    info = getInfoByString(ch,&len);

    if(isNull(info)){
        rInfo->err = 3;
        rInfo->errstr = dsnew("parse redis info error");
        return rInfo;
    }
    parseInfo(rInfo,info,len);
    return rInfo;
}


