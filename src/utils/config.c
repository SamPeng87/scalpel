/*
 * =====================================================================================
 *
 *       Filename:  config.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014/07/11 18时07分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "config.h"
#include <stdlib.h>

Config *IntConfigObj(int argc,char const *argv[], char const *configName){
    Config *config = calloc(1,sizeof(Config));
    return config;
}

