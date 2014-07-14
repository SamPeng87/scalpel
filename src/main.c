/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: 
 *
 *        Version:  1.0
 *        Created:  2014/07/11 13时27分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sampeng(penglingjun) (), penglingjun@baofeng.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "utils/config.h"
#include "redis/redis.h"

int main(int argc, char const* argv[])
{
    Config *config = IntConfigObj(argc,argv," ");
    redisRun(config);
    return 0;
}
