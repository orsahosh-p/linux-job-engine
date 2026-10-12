#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include <stdio.h>
#include "job.h"

typedef struct JobNode{
	Job* job;
	struct JobNode * next;
}JobNode;

typedef struct JobQueue{
	JobNode * head;
	JobNode * tail;
	size_t size;
}JobQueue;

JobQueue* job_queue_create(void);
bool job_queue_enqueue(JobQueue* queue, Job* job);
Job* job_queue_dequeue(JobQueue* queue);
Job* job_queue_peek(const JobQueue* queue);
void job_queue_destroy(JobQueue* queue);

#endif
