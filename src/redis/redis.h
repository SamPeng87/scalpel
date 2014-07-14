/*
 * =====================================================================================
 *
 *       Filename:  redis.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014/07/11 18时29分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _REDIS_MAIN_H
#define _REDIS_MAIN_H
#include "../utils/config.h"

typedef struct RedisConfig {
    const char *host;
    int port;
} RedisConfig;

void redisRun(Config *config);
#endif
