#include <stdio.h>
#include "job.h"
#include "job_vector.h"
#include "job_queue.h"
#include "scheduler.h"

int main(void){

	printf("Linux Job Engine\n");

	JobVector* vector = job_vector_create(3);
	JobQueue* queue = job_queue_create();

	if(vector == NULL || queue == NULL){
		job_queue_destroy(queue);
		job_vector_destroy(vector);
		return 1;
	}

	const char* commands[] = {
		"true",
		"false",
		"true"
	};

	for(size_t i = 0; i < 3; i++){
		Job* job = job_create(i + 1, commands[i], 5);

		if(job == NULL){
			goto cleanup;
		}

		if(!job_vector_push(vector, job)){
			job_destroy(job);
			goto cleanup;
		}

		if(!job_queue_enqueue(queue, job)){
			goto cleanup;
		}
	}

	Scheduler *scheduler = scheduler_create(queue);

	if(scheduler == NULL){
		goto cleanup;
	}

	printf("Starting FCFS Scheduler...\n\n");

	int result = scheduler_run_fcfs(scheduler);

	printf("\n===== RESULTS =====\n");

	for(size_t i = 0; i < vector->size; i++){
		Job *job = job_vector_get(vector, i);
		job_print(job);
	}

	printf("\nScheduler result: %d\n", result);

	scheduler_destroy(scheduler);
	job_queue_destroy(queue);
	job_vector_destroy(vector);

	return result;


cleanup:
	job_queue_destroy(queue);
	job_vector_destroy(vector);
	return 1;
}
