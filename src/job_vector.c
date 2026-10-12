#include "job_vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "job.h"

void job_vector_destroy(JobVector* vector){
	if(vector == NULL){
		return;
	}

	for(size_t i = 0; i < vector->size; i++){
		job_destroy(vector->data[i]);
	}
	free(vector->data);
	free(vector);
}

JobVector* job_vector_create(size_t initial_capacity){

	JobVector* vector = malloc(sizeof(JobVector));

	if(vector == NULL){
		return NULL;
	}

	if(initial_capacity > 0){

		if(initial_capacity > SIZE_MAX / sizeof(Job *)){
			free(vector);
			return NULL;
		}

		Job** jobs = calloc(initial_capacity, sizeof(Job*));
		if(jobs == NULL){
			free(vector);
			return NULL;
		}

		vector->data = jobs;

	}else if(initial_capacity == 0){
		vector->data = NULL;
	}

	vector->size = 0;
	vector->capacity = initial_capacity;

	return vector;
}

bool job_vector_push(JobVector* vector, Job* job){
	if(vector == NULL || job == NULL){
		return false;
	}

	if(vector->size == vector->capacity){

		size_t max_capacity = SIZE_MAX / sizeof(Job*);
		size_t new_capacity = vector->capacity;

		if(vector->capacity == 0){
			new_capacity = 1;
		}else{
			if(vector->capacity > max_capacity / 2){
				new_capacity = max_capacity;
			}else{
				new_capacity *= 2;
			}
		}

		Job** new_data = realloc(vector->data, new_capacity * sizeof(Job*));

		if(new_data == NULL){
			return false;
		}

		vector->data = new_data;
		vector->capacity = new_capacity;
	}

	vector->data[vector->size] = job;
	vector->size++;

	return true;
}

Job* job_vector_get(const JobVector* vector, size_t index){
	if(vector == NULL || index >= vector->size){
		return NULL;
	}

	return vector->data[index];
}

bool job_vector_remove(JobVector* vector, size_t index){
	if(vector == NULL || index >= vector->size){
		return false;
	}

	job_destroy(vector->data[index]);

	size_t i = index;

	while(i+1 < vector->size){
		vector->data[i] = vector->data[i+1];
		i++;
	}

	vector->data[i] = NULL;
	vector->size--;
	return true;
}
