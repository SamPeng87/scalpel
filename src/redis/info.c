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
#include <strings.h>
#include <stdio.h>

#include "info.h"

#define INFO_COMMAND "info all"
#define callocInfoMem(a){calloc(1,sizeof(a))}

char* getRedisInfoChar(redisContext *context) {
   const char* infoCommand = INFO_COMMAND; 
   redisReply* reply = (redisReply*) redisCommand(context,infoCommand);
   int len = strlen(reply->str);
   char *str = malloc(len+1);
   memcpy(str,reply->str,len);
   str[len+1] = '\0';
   freeReplyObject(reply);
   return str;
}

redisInfo *getRedisInfoObj(sdt ch){
    redisInfo *rInfo = callocInfoMem(redisInfo);
    if(NULL == ch){
        rInfo->err = 1;
        rInfo->errstr = "redis info is null";
        return rInfo;
    }
    printf("test1\n");
    if( (strncmp(ch,"ERR",2)) == 0){
        rInfo->err = 2;
        rInfo->errstr = strcat("redis info can't support command ",INFO_COMMAND);
        return rInfo;
    }

    printf("3\n");
    redisServerInfo *rServerInfo = callocInfoMem(redisServerInfo);
    redisClientInfo *rClientInfo = callocInfoMem(redisClientInfo);
    redisPersistenceInfo *rPersistenceInfo = callocInfoMem(redisPersistenceInfo);
    redisMemoryInfo *rMemoryInfo = callocInfoMem(redisMemoryInfo);
    redisStatsInfo *rStatsInfo = callocInfoMem(redisStatsInfo);
    redisCPUInfo *rCPUInfo = callocInfoMem(redisCPUInfo);
    redisKeyspaceInfo *rKeyInfo = callocInfoMem(redisKeyspaceInfo);
    redisReplicationInfo *rReplicationInfo = callocInfoMem(redisReplicationInfo);

    return NULL;
}

