#include "engine.h"

#include <string.h>

bool engine_init(Engine *engine){
	if(engine == NULL){
		return false;
	}

	memset(engine, 0, sizeof(*engine));

	engine->jobs = job_vector_create(8);

	engine->heap = job_heap_create();

	if(engine->jobs == NULL || engine->heap == NULL){
		goto fail;
	}

	for(size_t i = 0; i < WORKER_COUNT; i++){
		engine->queues[i] = job_queue_create();

		if(engine->queues[i] == NULL){
			goto fail;
		}

		engine->schedulers[i] = scheduler_create(engine->queues[i]);

		if(engine->schedulers[i] == NULL){
			goto fail;
		}
	}

	if(pthread_mutex_init(&engine->heap_mutex, NULL) != 0){
		goto fail;
	}

	engine->mutex_initialized = true;

	if(pthread_cond_init(&engine->job_available, NULL) != 0){
		pthread_mutex_destroy(&engine->heap_mutex);
		goto fail;
	}

	engine->cond_initialized = true;

	engine->shutdown = false;

	for(size_t i = 0; i < WORKER_COUNT; i++){
		engine->contexts[i] = (WorkerContext) {
			.scheduler = engine->schedulers[i],
			.heap = engine->heap,
			.heap_mutex = &engine->heap_mutex,
			.job_available = &engine->job_available,
			.shutdown = &engine->shutdown,
			.result = 0
		};
	}

	return true;

fail:
	engine_destroy(engine);
	return false;

}

bool engine_add_job(Engine *engine, Job *job){
	if(engine == NULL || job == NULL){
		return false;
	}

	if(!job_vector_push(engine->jobs, job)){
		return false;
	}

	pthread_mutex_lock(&engine->heap_mutex);

	bool success = job_heap_push(engine->heap, job);

	if(success){
		pthread_cond_signal(&engine->job_available);
	}

	pthread_mutex_unlock(&engine->heap_mutex);

	return success;
}

int engine_run(Engine *engine){
	size_t started = 0;
	int result = 0;

	for(size_t i = 0; i < WORKER_COUNT; i++){
		if(pthread_create(
			&engine->threads[i],
			NULL,
			worker_run,
			&engine->contexts[i]
		) != 0){
			result = 1;
			break;
		}
		started++;
	}

	pthread_mutex_lock(&engine->heap_mutex);

	engine->shutdown = true;

	pthread_cond_broadcast(&engine->job_available);

	pthread_mutex_unlock(&engine->heap_mutex);

	for(size_t i = 0; i < started; i++){
		pthread_join(engine->threads[i], NULL);

		if(engine->contexts[i].result != 0){
			result = 1;
		}
	}

	return result;
}

void engine_destroy(Engine* engine){
	if(engine == NULL){
		return;
	}

	if(engine->cond_initialized){
		pthread_cond_destroy(&engine->job_available);
	}

	if(engine->mutex_initialized){
		pthread_mutex_destroy(&engine->heap_mutex);
	}


	for(size_t i = 0; i < WORKER_COUNT; i++){
		scheduler_destroy(engine->schedulers[i]);
		job_queue_destroy(engine->queues[i]);
	}


	job_heap_destroy(engine->heap);
	job_vector_destroy(engine->jobs);

}
