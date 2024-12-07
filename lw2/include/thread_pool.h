#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <stdbool.h>

typedef struct {
    void (*function)(void*);
    void* arg;
} Task;

typedef struct {
    pthread_t* threads;
    Task* tasks;
    int thread_count;
    int task_count;
    int task_capacity;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool stop;
} ThreadPool;

void thread_pool_init(ThreadPool* pool, int thread_count);
void thread_pool_enqueue(ThreadPool* pool, void (*function)(void*), void* arg);
void thread_pool_wait(ThreadPool* pool);
void thread_pool_destroy(ThreadPool* pool);

#endif // THREAD_POOL_H