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
#include "../utils/cmalloc.h"

#define INFO_COMMAND "info"

char *getRedisInfoChar(redisContext *context) {
   const char* infoCommand = INFO_COMMAND; 
   redisReply* reply = (redisReply*) redisCommand(context,infoCommand);
   int len = strlen(reply->str);
   char *str = cmalloc(len+1);
   memcpy(str,reply->str,len);
   str[len+1] = '\0';
   freeReplyObject(reply);
   return str;
}

redisInfo *getRedisInfoObj(sds ch){
    redisInfo *rInfo = ccalloc(sizeof(redisInfo));
    if(NULL == ch){
        rInfo->err = 1;
        rInfo->errstr = "redis info is null";
        return rInfo;
    }
    if( (strncmp(ch,"ERR",2)) == 0){
        rInfo->err = 2;
        rInfo->errstr = strcat("redis info can't support command ",INFO_COMMAND);
        return rInfo;
    }

    printf("3\n");
    redisServerInfo *rServerInfo = ccalloc(sizeof(redisServerInfo));
    redisClientInfo *rClientInfo = ccalloc(sizeof(redisClientInfo));
    redisPersistenceInfo *rPersistenceInfo = ccalloc(sizeof(redisPersistenceInfo));
    redisMemoryInfo *rMemoryInfo = ccalloc(sizeof(redisMemoryInfo));
    redisStatsInfo *rStatsInfo = ccalloc(sizeof(redisStatsInfo));
    redisCPUInfo *rCPUInfo = ccalloc(sizeof(redisCPUInfo));
    redisKeyspaceInfo *rKeyInfo = ccalloc(sizeof(redisKeyspaceInfo));
    redisReplicationInfo *rReplicationInfo = ccalloc(sizeof(redisReplicationInfo));

    rInfo->redisServerInfo = rServerInfo;
    rInfo->redisCPUInfo = rCPUInfo;
    rInfo->redisClientInfo = rClientInfo;
    rInfo->redisPersistenceInfo = rPersistenceInfo;
    rInfo->redisMemoryInfo = rMemoryInfo;
    rInfo->redisStatsInfo = rStatsInfo;
    rInfo->redisKeyspaceInfo = rKeyInfo;
    rInfo->redisReplicationInfo = rReplicationInfo;

    

    printf("test1 %zu;%zu;%zu\n",cmalloc_used_memory(),cmalloc_get_rss(),cmalloc_get_private_dirty());
    cfree(ch);
    cfree(rStatsInfo);
    printf("test1 %zu;%zu;%zu\n",cmalloc_used_memory(),cmalloc_get_rss(),cmalloc_get_private_dirty());

    return NULL;
}

