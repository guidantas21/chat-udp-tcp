#ifndef POOL_H
#define POOL_H

#include <stdlib.h>
#include <pthread.h>

typedef struct 
{
    pthread_t *threads;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int num_threads;
    int stop; 
} ThreadPool;

void *thread_function(void *arg);

int thread_pool_init(ThreadPool *pool, const int num_threads , int server_socket);

int thread_pool_destroy(ThreadPool *pool);

#endif
