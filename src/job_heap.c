#include "job_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include "job.h"

JobHeap* job_heap_create(void){
	JobHeap *heap = malloc(sizeof(JobHeap));

	if(heap == NULL){
		return NULL;
	}

	heap->capacity = 4;
	heap->size = 0;

	heap->data =  malloc(heap->capacity * sizeof *heap->data);

	if(heap->data == NULL){
		free(heap);
		return NULL;
	}

	return heap;
}

void job_heap_destroy(JobHeap *heap){
	if(heap == NULL){
		return;
	}

	free(heap->data);
	free(heap);
}

bool job_heap_push(JobHeap *heap, Job *job){
	if(heap == NULL || job == NULL){
		return false;
	}

	if(heap->size == heap->capacity){

		if(heap->capacity > SIZE_MAX / sizeof *heap->data / 2){
			return false;
		}

		size_t new_capacity = heap->capacity * 2;

		Job** temp = realloc(heap->data, new_capacity * sizeof *heap->data);

		if(temp == NULL){
			return false;
		}

		heap->data = temp;
		heap->capacity = new_capacity;
	}

	size_t index = heap->size;

	heap->data[index] = job;

	while(index > 0){

		size_t parent = (index-1)/2;
		if(heap->data[parent]->priority >= heap->data[index]->priority){
			break;
		}
		Job* temp = heap->data[parent];
		heap->data[parent] = heap->data[index];
		heap->data[index] = temp;

		index = parent;
	}

	heap->size++;

	return true;
}

Job* job_heap_pop(JobHeap *heap){
	if(heap == NULL || heap->size == 0){
		return NULL;
	}

	Job* root = heap->data[0];

	heap->size--;

	if(heap->size == 0){
		heap->data[0] = NULL;
		return root;
	}

	heap->data[0] = heap->data[heap->size];
	heap->data[heap->size] = NULL;

	size_t index = 0;

	while(1){
		size_t left = 2*index + 1;
		size_t right = 2*index + 2;
		size_t largest = index;

		if(left < heap->size && heap->data[left]->priority > heap->data[largest]->priority){
			largest = left;
		}

		if(right < heap->size && heap->data[right]->priority > heap->data[largest]->priority){
			largest = right;
		}

		if(largest == index){
			break;
		}

		Job* temp = heap->data[index];
		heap->data[index] = heap->data[largest];
		heap->data[largest] = temp;

		index = largest;
	}

	return root;
}

Job* job_heap_peek(const JobHeap *heap){
	if(heap == NULL || heap->size <= 0){
		return NULL;
	}

	return heap->data[0];
}

