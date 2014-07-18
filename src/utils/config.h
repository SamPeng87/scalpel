/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  config headers
 *
 *        Version:  1.0
 *        Created:  2014/07/11 18时06分31秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _UTILS_CONFIG_H
#define _UTILS_CONFIG_H

#define isNull(o) (NULL==o)
#define GET_ARRAY_LEN(array,len) {len = (sizeof(array) / sizeof(array[0]));} 

typedef struct Config {
    char *redisInfoFile;
} Config;

Config *IntConfigObj(int argc,char const *argv[],char const *configName);
#endif
