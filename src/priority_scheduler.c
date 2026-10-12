#include "priority_scheduler.h"
#include "job_heap.h"
#include "scheduler.h"
#include <stdlib.h>

int priority_scheduler_run(Scheduler* scheduler, JobHeap *heap){
	if(scheduler == NULL || scheduler->queue == NULL || heap == NULL){
		return 1;
	}

	while(job_heap_peek(heap) != NULL){
		Job *job = job_heap_peek(heap);

		if(!job_queue_enqueue(scheduler->queue, job)){
			return 1;
		}

		job_heap_pop(heap);
	}

	return scheduler_run_fcfs(scheduler);
}
