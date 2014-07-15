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


typedef struct redisServerInfo{
    sdt version;
    sdt git_sha1;
    long int git_dirty;
    bool arch_bits;
    sdt multiplexing_api;
    unsigned short process_id;
    unsigned short run_id;
    time_t   uptime_in_seconds;
    time_t   uptime_in_days;
    int      lru_clock;
    sdt config_file;
    int tcp_port;
    
} redisServerInfo;

typedef struct redisClientInfo {
    unsigned short connected;
    int longest_output_list;
    int longest_input_buf;
    unsigned short blocked;
} redisClientInfo;

typedef struct redisMemoryInfo {
    long long used_memory;
    long long used_memory_rss;
    long long used_memory_peak;
    long long used_memory_lua;
    float mem_fragmentation_ratio;
    sdt used_memory_human;
    sdt used_memory_peak_human;
    sdt mem_allocator;
} redisMemoryInfo;

typedef struct redisPersistenceInfo{
    int loadding;
    long long rdb_changes_since_last_save;    
    bool rdb_bgsave_in_progress;
    time_t rdb_last_save_time;
    sdt rdb_last_bgsave_status;
    time_t rdb_last_bgsave_time_sec;
    time_t rdb_current_bgsave_time_sec;
    bool    aof_enabled;
    bool    aof_rewrite_in_progress;
    int    aof_rewrite_scheduled;
    time_t aof_last_rewrite_time_sec;
    time_t aof_current_rewrite_time_sec;
    sdt aof_last_bgrewrite_status;
    sdt aof_last_write_status;
    long long aof_current_size;
    long long aof_base_size;
    int aof_pending_rewrite;
    size_t aof_bufleng;
    unsigned long aof_rewrite_buffer_length;
    unsigned long long aof_pending_bio_fsync;
    unsigned long aof_delayed_fsync;
    time_t loading_start_time;
    unsigned long long loading_total_bytes;
    unsigned long long loading_loaded_bytes;
    double loading_loaded_perc;
    time_t loading_eta_seconds;
} redisPersistenceInfo;

typedef struct redisStatsInfo{
    long long total_connections_received;
    long long total_commands_processed;
    long long instantaneous_ops_per_sec;
    long long rejected_connections;
    long long sync_full;
    long long sync_partial_ok;
    long long sync_partial_err;
    long long expired_keys;
    long long evicted_keys;
    long long keyspace_hits;
    long long keyspace_misses;
    unsigned long pubsub_channels;
    unsigned long pubsub_patterns;
    long long latest_fork_usec;
} redisStatsInfo;

typedef struct redisSlavesInfo {
    sdt ip;
    int port;
    sdt state;
    long long offset;
    long lag;
} redisSlavesInfo;

typedef struct redisReplicationInfo{
    sdt role;
    sdt master_host;
    int port;
    sdt master_link_status;
    int master_last_io_seconds_ago;
    bool master_sync_in_progress;
    unsigned long long slave_repl_offset;
    long long master_sync_left_bytes;
    int master_sync_last_io_seconds_ago;
    time_t master_link_down_since_seconds;
    int slave_priority;
    int slave_read_only;
    unsigned connected_slaves;
    int min_slaves_good_slaves;
    long long master_repl_offset;
    bool repl_backlog_active;
    long long repl_backlog_size;
    long long repl_backlog_first_byte_offset;
    long long repl_backlog_histlen;
    redisSlavesInfo *slaves[];
} redisReplicationInfo;

typedef struct redisCPUInfo {
   float used_cpu_sys;
   float used_cpu_user;
   float used_cpu_sys_children;
   float used_cpu_user_children;
} redisCPUInfo;

typedef struct redisKeyInfo {
    int dbnum;
    long long keys;
    long long vkeys;
    long long avg_ttl; 
} redisKeyInfo;

typedef struct redisKeyspaceInfo{
    int dbnum;
    redisKeyInfo *keys[]; //C99 Only
} redisKeyspaceInfo;


typedef struct redisInfo{
    int err;
    sdt errstr;
    redisServerInfo *redisServerInfo;
    redisClientInfo *redisClientInfo;
    redisMemoryInfo *redisMemoryInfo;
    redisPersistenceInfo *redisPersistenceInfo;
    redisStatsInfo *redisStatsInfo;
    redisReplicationInfo *redisReplicationInfo;
    redisCPUInfo *redisCPUInfo;
    redisKeyspaceInfo *redisKeyspaceInfo;
} redisInfo;


char *getRedisInfoChar(redisContext *context);

redisInfo *getRedisInfoObj(sds ch);
#endif
