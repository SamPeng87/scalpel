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
#include <string.h>
#include <stdio.h>

#include "info.h"
#include "../utils/cmalloc.h"
#include "../utils/strings.h"
#include "../utils/config.h"

#define parseKeyAndValue(s,key,value) \
    int len = 0; \
    while(*s){ \
        if(*s == ':'){ \
            *s = '\0'; \
            key = s-len; \
            value = s+1; \
        } \
        len ++; \
        ++s; \
    } \
;

#define INFO_COMMAND "info"
#define ITEMS_SLOTS 5 


#define assertKey(o) !strcmp(key,o)

static void *infoMemoryPoint;

static inline void *catol(void *ch){
    long *p = cmalloc(sizeof(long));
    *p = atol(ch);
    return p;
}

static inline void *catoll(void *ch){
    long *p = cmalloc(sizeof(long long));
    *p = atol(ch);
    return p;
}


static inline void *catoi(void *ch){
    int *p = cmalloc(sizeof(int));
    *p = atoi(ch);
    return p;
}

static inline void *catof(void *ch){
    int *p = cmalloc(sizeof(int));
    *p = atof(ch);
    return p;
}

struct redisInfoItem redisInfoItemTable[]={
    {"redis_version",NULL,NULL},
    {"redis_git_sha1",NULL,NULL},
    {"redis_git_dirty",NULL,catol},
    {"redis_build_id",NULL,NULL},
    {"redis_mode",NULL,NULL},
    {"os",NULL,NULL},
    {"arch_bits",NULL,NULL},
    {"multiplexing_api",NULL,NULL},
    {"gcc_version",NULL,NULL},
    {"process_id",NULL,catoi},
    {"run_id",NULL,catoi},
    {"tcp_port",NULL,catoi},
    {"uptime_in_seconds",NULL,catol},
    {"uptime_in_days",NULL,catol},
    {"hz",NULL,catoi},
    {"lru_clock",NULL,catoi},
    {"config_file",NULL,NULL},
    {"connected_clients",NULL,catoi},
    {"client_longest_output_list",NULL,catoi},
    {"client_biggest_input_buf",NULL,catoi},
    {"blocked_clients",NULL,catoi},
    {"used_memory",NULL,catoll},
    {"used_memory_human",NULL,NULL},
    {"used_memory_rss",NULL,catoll},
    {"used_memory_peak",NULL,catoll},
    {"used_memory_peak_human",NULL,catoll},
    {"used_memory_lua",NULL,catoll},
    {"mem_fragmentation_ratio",NULL,catof},
    {"mem_allocator",NULL,NULL},
    {"loading",NULL,catoi},
    {"rdb_changes_since_last_save",NULL,catoll},
    {"rdb_bgsave_in_progress",NULL,catoi},
    {"rdb_last_save_time",NULL,catol},
    {"rdb_last_bgsave_status",NULL,NULL},
    {"rdb_last_bgsave_time_sec",NULL,catol},
    {"rdb_current_bgsave_time_sec",NULL,catol},
    {"aof_enabled",NULL,catoi},
    {"aof_rewrite_in_progress",NULL,catoi},
    {"aof_rewrite_scheduled",NULL,catoi},
    {"aof_last_rewrite_time_sec",NULL,catol},
    {"aof_current_rewrite_time_sec",NULL,catol},
    {"aof_last_bgrewrite_status",NULL,NULL},
    {"aof_last_write_status",NULL,NULL},

    {"total_connections_received",NULL,catoll},
    {"total_commands_processed",NULL,catoll},
    {"instantaneous_ops_per_sec",NULL,catoll},
    {"rejected_connections",NULL,catoll},
    {"sync_full",NULL,catoll},
    {"sync_partial_ok",NULL,catoll},
    {"sync_partial_err",NULL,catoll},
    {"expired_keys",NULL,catoll},
    {"evicted_keys",NULL,catoll},
    {"keyspace_hits",NULL,catoll},
    {"keyspace_misses",NULL,catoll},
    {"pubsub_channels",NULL,catoll},
    {"pubsub_patterns",NULL,catoll},
    {"latest_fork_usec",NULL,catoll},
    {"used_cpu_sys",NULL,catof},
    {"used_cpu_user",NULL,catof},
    {"used_cpu_sys_children",NULL,catof},
    {"used_cpu_user_children",NULL,catof},
    {"role",NULL,NULL},
    {"master_host",NULL,NULL},
    {"master_port",NULL,catoi},
    {"master_link_status",NULL,NULL},
    {"master_last_io_seconds_ago",NULL,catoi},
    {"master_sync_in_progress",NULL,catoi},
    {"slave_repl_offset",NULL,catoll},
    {"master_sync_left_bytes",NULL,catoll},
    {"master_sync_last_io_seconds_ago",NULL,catoi},
    {"master_link_down_since_seconds",NULL,catol},
    {"slave_priority",NULL,catoi},
    {"slave_read_only",NULL,catoi},
    {"connected_slaves",NULL,catoi},
    {"min_slaves_good_slaves",NULL,catoi},
    {"master_repl_offset",NULL,catoll},
    {"repl_backlog_active",NULL,catoi},
    {"repl_backlog_size",NULL,catoll},
    {"repl_backlog_first_byte_offset",NULL,catoll},
    {"repl_backlog_histlen",NULL,catoll},
};


redisReply *getRedisInfoChar(redisContext *context) {
   const char* infoCommand = INFO_COMMAND; 
   return (redisReply*) redisCommand(context,infoCommand);
}

char **getInfoByString(dst ch,int *count){
    int slots =5;
    char **splits = cmalloc(sizeof(char *)*slots);

    int i=0;
    int len=0;
    int start=0;
    do{
        if(*ch == '\n'){
            if(slots < i+2){
                char **newsplits;
                slots *= 2;
                newsplits = crealloc(splits,sizeof(char *)*slots);
                if(isNull(newsplits)) goto cleanup;
                splits = newsplits;
            }
            *ch = '\0';

            if(start == 0 ){
                splits[i] = ch-(len-start);
            }else{
                splits[i] = ch-(len-start-1);
            }
            start = len;
            i++;
        }
        len ++;
        ++ch;
    }while(*ch);
    *count = i;
    return splits;
cleanup :
    {
        cfree(splits);
        *count =0;
        return NULL;
    }
}

void switchValueOfObj(redisInfo *rInfo,char *key,char *value){
    int num = 0;
    GET_ARRAY_LEN(redisInfoItemTable,num);
    int j = 0;

    for(;j < num;j++){
        struct redisInfoItem *item = redisInfoItemTable+j;
        if(assertKey(item->name)){
            if(isNull(item->transform)){
                int strl = sizeof(char)*strlen(value);
                char *newvalue = cmalloc(strl+1);
                memcpy(newvalue,value,strlen(value));
                newvalue[strl]='\0';
                item->value = value;
            }else{
                item->value = item->transform(value);
            }
            if(item != NULL){
                rInfo->items[rInfo->itemslen] = item;
                rInfo->itemslen++;
            }
        }
    }

}

void parseInfo(redisInfo *rInfo,char **info,int count){
    int j=0;
    for(;j< count;j++){
        char *rik = info[j]; 
        char *key = NULL;
        char *value = NULL;
        parseKeyAndValue(rik,key,value);
        switchValueOfObj(rInfo,key,value);
    }
    redisInfoItems *t = rInfo->items;
    int len = rInfo->itemslen;
    int n=0;
    for(;n<len;n++){
        t = rInfo->items+n;
        if((*t) != NULL){
            if(isNull((*t)->transform)){
                printf("%s=%s\n",(*t)->name,(*t)->value);
            }else{
                printf("%s=%d\n",(*t)->name,*(int*)((*t)->value));
            }
        }
    }
}

redisInfo *getRedisInfoObj(dst ch){
    redisInfo *rInfo = ccalloc(sizeof(redisInfo));
    int itemCount = 0;
    GET_ARRAY_LEN(redisInfoItemTable,itemCount);
    rInfo->items = ccalloc(sizeof(redisInfoItem)*itemCount);
    rInfo->slaves = ccalloc(sizeof(redisSlavesInfo)*ITEMS_SLOTS);
    rInfo->keys   = ccalloc(sizeof(redisKeyInfo)*ITEMS_SLOTS);
    rInfo->commands = ccalloc(sizeof(redisCommandInfo)*ITEMS_SLOTS);

    char** info;
    if(NULL == ch){
        rInfo->err = 1;
        rInfo->errstr = "redis info is null";
        return rInfo;
    }


    if( (strncmp(ch,"ERR",2)) == 0){
        rInfo->err = 2;
        dst s= dsnew("redis info can't support command:");
        rInfo->errstr = dscat(s,INFO_COMMAND);
        return rInfo;
    }


    int len=0;
    info = getInfoByString(ch,&len);

    if(isNull(info)){
        rInfo->err = 3;
        rInfo->errstr = dsnew("parse redis info error");
        return rInfo;
    }
    parseInfo(rInfo,info,len);
    return rInfo;
}
