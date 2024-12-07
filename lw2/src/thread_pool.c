#include "thread_pool.h"
#include <stdlib.h>
#include <stdio.h>

// Рабочий поток, который берет задачи из очереди и выполняет их
void* thread_pool_worker(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    while (true) {
        pthread_mutex_lock(&pool->mutex);
        while (!pool->stop && pool->task_count == 0) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }
        if (pool->stop && pool->task_count == 0) {
            pthread_mutex_unlock(&pool->mutex);
            return NULL;
        }
        Task task = pool->tasks[--pool->task_count];
        pthread_mutex_unlock(&pool->mutex);
        task.function(task.arg);
    }
}

// Инициализация пула потоков
void thread_pool_init(ThreadPool* pool, int thread_count) {
    pool->threads = malloc(thread_count * sizeof(pthread_t));
    pool->tasks = malloc(1000 * sizeof(Task));
    pool->thread_count = thread_count;
    pool->task_count = 0;
    pool->task_capacity = 1000;
    pool->stop = false;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    for (int i = 0; i < thread_count; ++i) {
        pthread_create(&pool->threads[i], NULL, thread_pool_worker, pool);
    }
}

// Добавление задачи в очередь пула потоков
void thread_pool_enqueue(ThreadPool* pool, void (*function)(void*), void* arg) {
    pthread_mutex_lock(&pool->mutex);
    if (pool->task_count == pool->task_capacity) {
        pool->task_capacity *= 2;
        pool->tasks = realloc(pool->tasks, pool->task_capacity * sizeof(Task));
        if (!pool->tasks) {
            perror("Failed to reallocate memory for tasks");
            pthread_mutex_unlock(&pool->mutex);
            return;
        }
    }
    pool->tasks[pool->task_count++] = (Task){function, arg};
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
}

// Ожидание завершения всех задач в очереди
void thread_pool_wait(ThreadPool* pool) {
    while (true) {
        pthread_mutex_lock(&pool->mutex);
        int task_count = pool->task_count;
        pthread_mutex_unlock(&pool->mutex);
        if (task_count == 0) {
            break;
        }
    }
}

// Уничтожение пула потоков и освобождение ресурсов
void thread_pool_destroy(ThreadPool* pool) {
    pthread_mutex_lock(&pool->mutex);
    pool->stop = true;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
    for (int i = 0; i < pool->thread_count; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);
    free(pool->tasks);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
}