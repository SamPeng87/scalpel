/*
 * =====================================================================================
 *
 *       Filename:  info.h
 *
 *    Description:  redis info
 *
 *        Version:  1.0
 *        Created:  2014/07/11 14时21分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _REDIS_INFO_H
#define _REDIS_INFO_H
#include <hiredis/hiredis.h>
#include <time.h>
#include <stdbool.h>
#include "../utils/strings.h"
#include "../utils/config.h"

typedef struct redisInfoItem{
    const char *name;
    void *value;
    void *(*transform)(void *ch);
    char type;
    int index;
} redisInfoItem;

typedef struct redisSlavesInfo {
    cdst ip;
    int port;
    cdst state;
    long long offset;
    long lag;
} redisSlavesInfo;


typedef struct redisKeyInfo {
    int dbnum;
    long long keys;
    long long vkeys;
    long long avg_ttl; 
} redisKeyInfo;

typedef struct redisCommandInfo{
    long long calls;
    long long usec;
    float usecPerCall;
} redisCommandInfo;



typedef redisInfoItem* redisInfoItems;
typedef redisKeyInfo* redisKeyInfos;
typedef redisSlavesInfo* redisSlavesInfos;
typedef redisCommandInfo* redisCommandInfos;

typedef struct redisInfo{
    int err;
    dst errstr;
    time_t time;
    redisInfoItems   items;
    int itemslen;
    redisSlavesInfos *slaves;
    int slaveslen;
    redisKeyInfos    *keys;
    int keyslen;
    redisCommandInfos *commands;
    int commandLen;
} redisInfo;


redisReply *getRedisInfoChar(redisContext *context);

redisInfo *getRedisInfoObj(dst ch);

int writeInfo(Config *c,redisInfo *r);
void readListInfo(Config *c,int size,long *count);
#endif
