#ifndef JOB_HEAP_H
#define JOB_HEAP_H

#include <stddef.h>
#include "job.h"

typedef struct {
	Job** data;
	size_t size;
	size_t capacity;
}JobHeap;

JobHeap *job_heap_create(void);
void job_heap_destroy(JobHeap *heap);
bool job_heap_push(JobHeap *heap, Job *job);
Job* job_heap_pop(JobHeap* heap);

#endif
