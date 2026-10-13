#ifndef WORKER_H
#define WORKER_H

#include "scheduler.h"
#include "job_heap.h"
#include <pthread.h>
#include <stdbool.h>

typedef struct {
	Scheduler *scheduler;
	JobHeap * heap;

	pthread_mutex_t *heap_mutex;
	pthread_cond_t *job_available;

	bool *shutdown;

	int result;
} WorkerContext;

void *worker_run(void *arg);

#endif
