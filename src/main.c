#include <stdio.h>
#include "job.h"
#include "job_vector.h"
#include "job_queue.h"
#include "scheduler.h"
#include "priority_scheduler.h"

int main(void){

	printf("Linux Job Engine\n");

	JobVector* vector = job_vector_create(3);
	JobQueue* queue = job_queue_create();
	JobHeap* heap = job_heap_create();

	if(vector == NULL || queue == NULL || heap == NULL){
		job_queue_destroy(queue);
		job_vector_destroy(vector);
		job_heap_destroy(heap);
		return 1;
	}

	const char* commands[] = {
		"true",
		"false",
		"true"
	};

	int priorities[] = {3, 9, 5};

	for(size_t i = 0; i < 3; i++){
		Job* job = job_create(i + 1, commands[i], priorities[i]);

		if(job == NULL){
			goto cleanup;
		}

		if(!job_vector_push(vector, job)){
			job_destroy(job);
			goto cleanup;
		}

		if(!job_heap_push(heap, job)){
			goto cleanup;
		}
	}

	Scheduler *scheduler = scheduler_create(queue);

	if(scheduler == NULL){
		goto cleanup;
	}

	printf("Starting Priority + FCFS Scheduler...\n\n");

	int result = priority_scheduler_run(scheduler, heap);

	printf("\n===== RESULTS =====\n");

	for(size_t i = 0; i < vector->size; i++){
		Job *job = job_vector_get(vector, i);
		job_print(job);
	}

	printf("\nScheduler result: %d\n", result);

	scheduler_destroy(scheduler);
	job_queue_destroy(queue);
	job_vector_destroy(vector);
	job_heap_destroy(heap);

	return result;


cleanup:
	job_queue_destroy(queue);
	job_vector_destroy(vector);
	job_heap_destroy(heap);
	return 1;
}
