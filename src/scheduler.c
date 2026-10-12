#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "job_queue.h"
#include "scheduler.h"
#include "job.h"
#include "process.h"

Scheduler *scheduler_create(JobQueue *queue){
	if(queue == NULL){
		return NULL;
	}

	Scheduler* scheduler = malloc(sizeof(Scheduler));

	if(scheduler == NULL){
		return NULL;
	}

	scheduler->queue = queue;
	scheduler->current = NULL;

	return scheduler;
}

void scheduler_destroy(Scheduler *scheduler){
	if(scheduler == NULL){
		return;
	}

	free(scheduler);
}

int scheduler_run_fcfs(Scheduler *scheduler){
	if(scheduler == NULL || scheduler->queue == NULL){
		return 1;
	}

	int failed = 0;

	while(scheduler->queue->size > 0){

		Job* job = job_queue_dequeue(scheduler->queue);

		printf("[SCHEDULER] Starting Job %lu (priority=%d)\n", (unsigned long)job->id, job->priority);

		if(job == NULL){
			return 1;
		}

		if(!job_set_state(job, JOB_RUNNING)){
			return 1;
		}

		scheduler->current = job;

		char* argv[] = {job->command, NULL};

		pid_t pid = process_start(job->command, argv);

		if(pid == -1){
			job_set_state(job, JOB_FAILED);
			scheduler->current = NULL;
			failed = 1;
			continue;
		}

		job->pid = pid;

		int result;
		int exit_code = -1;

		do{
			result = process_check(pid, &exit_code);
			if(result == 0){
				sleep(1);
			}
		}while(result == 0);

		if(result == -1){
			job_set_state(job, JOB_FAILED);
			scheduler->current = NULL;
			return 1;
		}

		job->exit_code = exit_code;

		if(exit_code == 0){
			if(!job_set_state(job, JOB_COMPLETED)){
				scheduler->current = NULL;
				return 1;
			}
		}else{
			if(!job_set_state(job, JOB_FAILED)){
				scheduler->current = NULL;
				return 1;
			}

			failed = 1;
		}

		printf("[SCHEDULER] Finished Job %lu (exit_code=%d)\n", (unsigned long)job->id, job->exit_code);

		scheduler->current = NULL;
	}

	return failed;
}
