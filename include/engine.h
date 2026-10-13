#ifndef ENGINE_H
#define ENGINE_H

#include "job_vector.h"
#include "job_heap.h"
#include "job_queue.h"
#include "scheduler.h"
#include "worker.h"
#include <stdbool.h>

#define WORKER_COUNT 2

typedef struct {

	JobVector* jobs;
	JobHeap* heap;

	JobQueue* queues[WORKER_COUNT];
	Scheduler* schedulers[WORKER_COUNT];

	pthread_t threads[WORKER_COUNT];
	WorkerContext contexts[WORKER_COUNT];

	pthread_mutex_t heap_mutex;
	pthread_cond_t job_available;

	bool mutex_initialized;
	bool cond_initialized;

	bool shutdown;

} Engine;

bool engine_init(Engine *engine);
int engine_run(Engine* engine);
bool engine_add_job(Engine *engine, Job *job);
void engine_print_results(const Engine* engine);
void engine_destroy(Engine* engine);

#endif
