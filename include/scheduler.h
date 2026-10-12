#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job_queue.h"

typedef struct {
	JobQueue *queue;
	Job *current;
}Scheduler;

Scheduler *scheduler_create(JobQueue *queue);
void scheduler_destroy(Scheduler *scheduler);
int scheduler_run_fcfs(Scheduler *scheduler);

#endif
