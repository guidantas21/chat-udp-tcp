#include "pool.h"

#include <stdio.h>
#include <errno.h>

#include "server.h"

void *thread_function(void *arg)
{
    Server *server = (Server *)arg;
    
    handle_client(server);

    return NULL;
}

int thread_pool_init(ThreadPool *pool, const int num_threads, int server_socket)
{
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond, NULL);
        
    pool->threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
    
    if (pool->threads == NULL)
    {
        perror("Error allocating memory for threads pool");
        return -1;
    }
    
    pool->num_threads = num_threads;

    for (int i = 0; i < pool->num_threads; ++i)
    {
        int thread = pthread_create(
            &pool->threads[i],
            NULL,
            thread_function,
            &server_socket
        );
        if (thread != 0)
        {
            perror("Error creating thread");
            return -1;
        }
    }
    return 0;
}

int thread_pool_destroy(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->lock);
    pool->stop = 1;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < pool->num_threads; ++i)
    {
        if (pthread_join(pool->threads[i], NULL) != 0)
        {
            perror("Error joinning thread");
            return -1;
        }
    }
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);

    free(pool->threads);
    pool->threads = NULL;

    return 0;
}
