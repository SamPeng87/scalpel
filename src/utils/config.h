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
typedef struct Config {
    int test;
} Config;

Config *IntConfigObj(int argc,char const *argv[],char const *configName);
#endif