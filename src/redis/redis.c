/*
 * =====================================================================================
 *
 *       Filename:  redis.c
 *
 *    Description:  redis main file
 *
 *        Version:  1.0
 *        Created:  2014/07/11 17时57分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <unistd.h>

#include "info.h"
#include "redis.h"
#include "../utils/config.h"
#include "../utils/cmalloc.h"
#include "../utils/strings.h"



RedisConfig *initRedisConfig(Config *config){
    RedisConfig *c;
    c = calloc(1,sizeof(RedisConfig));
    if(c == NULL){
        return NULL;
    }
    c->host = "127.0.0.1";
    c->port = 6379;
    return c;
}

void redisRun(Config *config){
    RedisConfig *redisConfig;
    redisConfig = initRedisConfig(config);
    /*readLastInfo(config);*/
    /*redisContext *redis = redisConnect(redisConfig->host,redisConfig->port);*/
    /*redisReply *infoReply = getRedisInfoChar(redis);*/
    /*printf("%s\n",infoReply->str);*/
    if(redisConfig != NULL){
        redisContext *redis = redisConnect(redisConfig->host,redisConfig->port); 
        redisReply *infoReply = getRedisInfoChar(redis); 
        redisInfo *infoObj = getRedisInfoObj(infoReply->str);
        if(infoObj->err){
            printf("get info error:%s\n",infoObj->errstr);
            dsfree(infoObj->errstr);
        }else{
            long c=0;
            readListInfo(config,3,&c);
            /*printf("%lu\n",c);*/
            /*writeInfo(config,infoObj);*/
            /*sleep(2);*/
            /*writeInfo(config,infoObj);*/
            /*sleep(2);*/
            /*writeInfo(config,infoObj);*/
        }
        freeReplyObject(infoReply);
    }
    printf("%zu\n",cmalloc_used_memory());
}


