#ifndef JOB_VECTOR_H
#define JOB_VECTOR_H

#include "job.h"
#include <stdbool.h>

typedef struct{
	Job** data;
	size_t size;
	size_t capacity;
} JobVector;

JobVector* job_vector_create(size_t initial_capacity);
bool job_vector_push(JobVector* vector, Job* job);
Job* job_vector_get(const JobVector* vector, size_t index);
bool job_vector_remove(JobVector *vector, size_t index);
void job_vector_destroy(JobVector *vector);

#endif
