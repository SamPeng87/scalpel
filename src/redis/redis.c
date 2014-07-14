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
#include <hiredis/hiredis.h>
#include <stdlib.h>

#include "info.h"
#include "redis.h"
#include "../utils/config.h"


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
    if(redisConfig != NULL){
        printf("redis host is %s ,port is %d",redisConfig->host,redisConfig->port);
        redisContext *redis = redisConnect(redisConfig->host,redisConfig->port); 
        char *infoChar = getRedisInfoChar(redis); 
        printf("start %s \n",infoChar);
        redisInfo *infoObj = getRedisInfoObj(infoChar);
        printf("end \n");
    }
}


