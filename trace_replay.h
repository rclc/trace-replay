#ifndef _TRACE_REPLAY_H
#define _TRACE_REPLAY_H

#include <stdio.h>
#include <libaio.h>
#include "flist.h"

#define USE_MAINWORKER 0

#define MB (1024*1024)
#define GB (1024*1024*1024)
#define MAX_QDEPTH 128
#define MAX_THREADS 128
#define STR_SIZE 128

#define PAGE_SIZE 4096
#define SECTOR_SIZE 512
#define SPP (PAGE_SIZE/SECTOR_SIZE)

struct io_stat_t{
	double latency_sum;
	unsigned int latency_count;
	unsigned long long total_operations;
	unsigned long long total_bytes;
	unsigned long long total_rbytes;
	unsigned long long total_wbytes;
	unsigned long long total_error_bytes;
	struct timeval start_time, end_time;
	double execution_time;
	int trace_repeat_count;
};

struct thread_info_t{
	int tid;

	struct flist_head queue;
	pthread_mutex_t mutex;
	pthread_cond_t cond_main, cond_sub;
	io_context_t io_ctx;
	struct io_event events[MAX_QDEPTH];

	struct io_stat_t io_stat;

	int queue_depth;
	int queue_count;
	int active_count;

	int fd;
	long long total_capacity;
	long long total_pages;
	long long total_sectors;
	long long start_partition;
	long long start_page;
	double timeout;

	FILE *trace_fp;
	char filename[STR_SIZE];

	int done;
};

struct io_job{
	struct iocb iocb;
	struct flist_head list;
    struct timeval start_time, stop_time;
	long long offset; // in bytes
	size_t bytes;
	int rw; // is read 
	char *buf;
};


typedef char                    __s8;   
typedef short                   __s16;  
typedef int                     __s32;  

#if TEST_OS == LINUX
typedef long long					__s64;
#endif 

typedef unsigned char           __u8;  
typedef unsigned short          __u16; 
typedef unsigned int            __u32; 

#if TEST_OS == LINUX
typedef unsigned long long			__u64;
#endif

#endif 
