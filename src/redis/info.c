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
#include <time.h>

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
#define TYPE_STRING 0
#define TYPE_INT 1
#define TYPE_LONG 2
#define TYPE_LONG_LONG 3
#define TYPE_FLOAT 4


#define assertKey(o) !strcmp(key,o)

static void *infoMemoryPoint;

static inline void *catol(void *ch){
    long *p = cmalloc(sizeof(long));
    *p = atol(ch);
    return p;
}

static inline void *catoll(void *ch){
    long long *p = cmalloc(sizeof(long long));
    *p = atol(ch);
    return p;
}


static inline void *catoi(void *ch){
    int *p = cmalloc(sizeof(int));
    *p = atoi(ch);
    return p;
}

static inline void *catof(void *ch){
    float *p = cmalloc(sizeof(float));
    *p = atof(ch);
    return p;
}

struct redisInfoItem redisInfoItemTable[]={
    {"redis_version",NULL,NULL,0,0},
    {"redis_git_sha1",NULL,NULL,0,0},
    {"redis_git_dirty",NULL,catol,TYPE_LONG,0},
    {"redis_build_id",NULL,NULL,0,0},
    {"redis_mode",NULL,NULL,0,0},
    {"os",NULL,NULL,0,0},
    {"arch_bits",NULL,NULL,0,0},
    {"multiplexing_api",NULL,NULL,0,0},
    {"gcc_version",NULL,NULL,0,0},
    {"process_id",NULL,catoi,TYPE_INT,0},
    {"run_id",NULL,catoi,TYPE_INT,0},
    {"tcp_port",NULL,catoi,TYPE_INT,0},
    {"uptime_in_seconds",NULL,catol,TYPE_LONG,0},
    {"uptime_in_days",NULL,catol,TYPE_LONG,0},
    {"hz",NULL,catoi,TYPE_INT,0},
    {"lru_clock",NULL,catoi,TYPE_INT,0},
    {"config_file",NULL,NULL,0,0},
    {"connected_clients",NULL,catoi,TYPE_INT,0},
    {"client_longest_output_list",NULL,catoi,TYPE_INT,0},
    {"client_biggest_input_buf",NULL,catoi,TYPE_INT,0},
    {"blocked_clients",NULL,catoi,TYPE_INT,0},
    {"used_memory",NULL,catoll,TYPE_LONG_LONG,0},
    {"used_memory_human",NULL,NULL,0,0},
    {"used_memory_rss",NULL,catoll,TYPE_LONG_LONG,0},
    {"used_memory_peak",NULL,catoll,TYPE_LONG_LONG,0},
    {"used_memory_peak_human",NULL,catoll,TYPE_LONG_LONG,0},
    {"used_memory_lua",NULL,catoll,TYPE_LONG_LONG,0},
    {"mem_fragmentation_ratio",NULL,catof,TYPE_FLOAT,0},
    {"mem_allocator",NULL,NULL,0,0},
    {"loading",NULL,catoi,TYPE_INT,0},
    {"rdb_changes_since_last_save",NULL,catoll,TYPE_LONG_LONG,0},
    {"rdb_bgsave_in_progress",NULL,catoi,TYPE_INT,0},
    {"rdb_last_save_time",NULL,catol,TYPE_LONG,0},
    {"rdb_last_bgsave_status",NULL,NULL,0,0},
    {"rdb_last_bgsave_time_sec",NULL,catol,TYPE_LONG,0},
    {"rdb_current_bgsave_time_sec",NULL,catol,TYPE_LONG,0},
    {"aof_enabled",NULL,catoi,TYPE_INT,0},
    {"aof_rewrite_in_progress",NULL,catoi,TYPE_INT,0},
    {"aof_rewrite_scheduled",NULL,catoi,TYPE_INT,0},
    {"aof_last_rewrite_time_sec",NULL,catol,TYPE_LONG,0},
    {"aof_current_rewrite_time_sec",NULL,catol,TYPE_LONG,0},
    {"aof_last_bgrewrite_status",NULL,NULL,0,0},
    {"aof_last_write_status",NULL,NULL,0,0},
    {"total_connections_received",NULL,catoll,TYPE_LONG_LONG,0},
    {"total_commands_processed",NULL,catoll,TYPE_LONG_LONG,0},
    {"instantaneous_ops_per_sec",NULL,catoll,TYPE_LONG_LONG,0},
    {"rejected_connections",NULL,catoll,TYPE_LONG_LONG,0},
    {"sync_full",NULL,catoll,TYPE_LONG_LONG,0},
    {"sync_partial_ok",NULL,catoll,TYPE_LONG_LONG,0},
    {"sync_partial_err",NULL,catoll,TYPE_LONG_LONG,0},
    {"expired_keys",NULL,catoll,TYPE_LONG_LONG,0},
    {"evicted_keys",NULL,catoll,TYPE_LONG_LONG,0},
    {"keyspace_hits",NULL,catoll,TYPE_LONG_LONG,0},
    {"keyspace_misses",NULL,catoll,TYPE_LONG_LONG,0},
    {"pubsub_channels",NULL,catoll,TYPE_LONG_LONG,0},
    {"pubsub_patterns",NULL,catoll,TYPE_LONG_LONG,0},
    {"latest_fork_usec",NULL,catoll,TYPE_LONG_LONG,0},
    {"used_cpu_sys",NULL,catof,TYPE_FLOAT,0},
    {"used_cpu_user",NULL,catof,TYPE_FLOAT,0},
    {"used_cpu_sys_children",NULL,catof,TYPE_FLOAT,0},
    {"used_cpu_user_children",NULL,catof,TYPE_FLOAT,0},
    {"role",NULL,NULL,0,0},
    {"master_host",NULL,NULL,0,0},
    {"master_port",NULL,catoi,TYPE_INT,0},
    {"master_link_status",NULL,NULL,0,0},
    {"master_last_io_seconds_ago",NULL,catoi,TYPE_INT,0},
    {"master_sync_in_progress",NULL,catoi,TYPE_INT,0},
    {"slave_repl_offset",NULL,catoll,TYPE_LONG_LONG,0},
    {"master_sync_left_bytes",NULL,catoll,TYPE_LONG_LONG,0},
    {"master_sync_last_io_seconds_ago",NULL,catoi,TYPE_INT,0},
    {"master_link_down_since_seconds",NULL,catol,TYPE_LONG,0},
    {"slave_priority",NULL,catoi,TYPE_INT,0},
    {"slave_read_only",NULL,catoi,TYPE_INT,0},
    {"connected_slaves",NULL,catoi,TYPE_INT,0},
    {"min_slaves_good_slaves",NULL,catoi,TYPE_INT,0},
    {"master_repl_offset",NULL,catoll,TYPE_LONG_LONG,0},
    {"repl_backlog_active",NULL,catoi,TYPE_INT,0},
    {"repl_backlog_size",NULL,catoll,TYPE_LONG_LONG,0},
    {"repl_backlog_first_byte_offset",NULL,catoll,TYPE_LONG_LONG,0},
    {"repl_backlog_histlen",NULL,catoll,TYPE_LONG_LONG,0},
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
            item->index = j;
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
                //memcpy
                redisInfoItem *newitem = rInfo->items + rInfo->itemslen;
                memcpy(newitem,item,sizeof(redisInfoItem));
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
}

redisInfo *getRedisInfoObj(dst ch){
    redisInfo *rInfo = ccalloc(sizeof(redisInfo));
    time_t t;
    time(&t);
    rInfo->time = t;
    printf("%zu\n",t);

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

void writeInfoItem(FILE *f,redisInfoItem *item,int *error){
    size_t size;
    switch(item->type){
        case TYPE_STRING    : size = strlen(item->value) ;break;
        case TYPE_INT       : size = sizeof(int);break;
        case TYPE_FLOAT     : size = sizeof(float);break;
        case TYPE_LONG_LONG : size = sizeof(long long);break;
        case TYPE_LONG      : size = sizeof(long);break;
    }
    if(item->type == TYPE_STRING){
        fwrite(&size,sizeof(size_t),1,f);
    }
    fwrite(item->value,size,1,f);
}

int writeInfo(Config *c,redisInfo *r){
    int len;
    len = r->itemslen;
    redisInfoItems items = r->items;
    FILE *f = fopen(c->redisInfoFile,"ab");

    fwrite(&len,sizeof(int),1,f);
    fwrite(&(r->time),sizeof(size_t),1,f);
    
    //write index;
    int j=0;
    for(;j<len;j++){
        int error=0;
        redisInfoItem *item = items+j;
        short namelen = item->index;
        fwrite(&namelen,sizeof(short),1,f);
    }

    j = 0;
    for(;j<len;j++){
        int error=0;
        redisInfoItem *item = items+j;
        writeInfoItem(f,item,&error);
    }
    fclose(f);
    return 1;
}


redisInfo *readLastInfo(FILE *f,int *readcount){
    int len = 0;
    int num = 0;
    int count = 0;
    redisInfo *r = cmalloc(sizeof(*r));

    
    //read item lens;
    fread(&len,sizeof(int),1,f);
    count += sizeof(int);
    r->itemslen = len;

    //read item time;
    time_t cTime; 
    fread(&cTime,sizeof(time_t),1,f);
    count += sizeof(time_t);

    r->time = cTime;    
    printf("%zu\n",cTime);

    //alloc memery
    redisInfoItem *block = cmalloc(sizeof(redisInfoItem)*len);

    //read item index;
    short index[len];
    fread(index,sizeof(short),len,f);
    count += sizeof(short)*len;

    
    GET_ARRAY_LEN(redisInfoItemTable,num);
    int j = 0;
    for(;j<len;j++){
        struct redisInfoItem *item = redisInfoItemTable+index[j];
        item->index = j;
        memcpy(block,item,sizeof(redisInfoItem));
        size_t size;
        switch(block->type){
            case TYPE_STRING    : 
                fread(&size,sizeof(size_t),1,f);
                count += sizeof(size_t);
                block->value = cmalloc(sizeof(char)*size);  ;break;
            case TYPE_INT       : size = sizeof(int);block->value = cmalloc(size);break;
            case TYPE_FLOAT     : size = sizeof(float);block->value = cmalloc(size);break;
            case TYPE_LONG_LONG : size = sizeof(long long);block->value = cmalloc(size);break;
            case TYPE_LONG      : size = sizeof(long);block->value = cmalloc(size);break;
        }

        fread(block->value,size,1,f);
        count += size;
        switch(block->type){
            case TYPE_STRING:
                printf("%d;%s=%s\n",block->type,block->name,block->value);
                break;
            case TYPE_FLOAT: 
                printf("%d;%s=%.2f\n",block->type,block->name,*(float *)block->value);
                break;
            case TYPE_LONG:
                printf("%d;%s=%lu\n",block->type,block->name,*(long *)block->value);
                break;
            case TYPE_LONG_LONG:
                printf("%d;%s=%llu\n",block->type,block->name,*(long long *)block->value);
                break;
            case TYPE_INT:
                printf("%d;%s=%d\n",block->type,block->name,*(int *)block->value);
                break;
        }
        block++;
    }

    block = block - len;
    r->items = block;
    *readcount = count; 
    return r;   
}

void readListInfo(Config *c,int size,long *count){
    if(!size) return;
    int i = 0;
    long readcount = 0;
    FILE *f = fopen(c->redisInfoFile,"rb");
    for(;i<size;i++){
        int readbuf=0;
        readLastInfo(f,&readbuf);
        printf("===============%d:%d:%lu============\n",i,readbuf,readcount);
        readcount += readbuf;
    }
    count = &readcount;
    printf("%lu\n",readcount);
    fclose(f);
}

