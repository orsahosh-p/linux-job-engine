#include "worker.h"
#include "priority_scheduler.h"

static Job* worker_take_job(WorkerContext *context);

void* worker_run(void* arg){
	WorkerContext* context = arg;

	if(context == NULL){
		return NULL;
	}

	context->result = 0;

	while(1){
		Job* job = worker_take_job(context);

		if(job == NULL){
			break;
		}

		if(!job_queue_enqueue(context->scheduler->queue, job)){
			context->result = 1;
			break;
		}

		int result = scheduler_run_fcfs(context->scheduler);

		if(result != 0){
			context->result = 1;
		}
	}

	return NULL;
}

static Job* worker_take_job(WorkerContext *context){
	pthread_mutex_lock(context->heap_mutex);

	while(job_heap_peek(context->heap) == NULL && !*context->shutdown){
		pthread_cond_wait(
			context->job_available,
			context->heap_mutex
		);

	}

	Job *job = job_heap_peek(context->heap);

	if(job != NULL){
		job_heap_pop(context->heap);
	}

	pthread_mutex_unlock(context->heap_mutex);

	return job;
}
