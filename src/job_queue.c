#include <stdio.h>
#include <stdlib.h>
#include "job_queue.h"
#include <stdbool.h>

JobQueue* job_queue_create(void){
	JobQueue* queue = malloc(sizeof(JobQueue));

	if(queue == NULL){
		return NULL;
	}

	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;

	return queue;
}

Job* job_queue_peek(const JobQueue* queue){
	if(queue == NULL || queue->size == 0 || queue->head->job == NULL){
		return NULL;
	}

	return queue->head->job;
}

void job_queue_destroy(JobQueue* queue){
	if(queue == NULL){
		return;
	}

	JobNode* current = queue->head;

	while(current != NULL){
		JobNode* next = current->next;

		free(current);

		current = next;
	}

	free(queue);
}

bool job_queue_enqueue(JobQueue* queue, Job* job){
	if(queue == NULL || job == NULL){
		return false;
	}

	JobNode* node = malloc(sizeof(JobNode));
	if(node == NULL){
		return false;
	}

	node->job = job;
	node->next = NULL;

	if(queue->size == 0){
		queue->head = node;
		queue->tail = node;
	}else{
		queue->tail->next = node;
		queue->tail = node;
	}

	queue->size++;

	return true;
}

Job* job_queue_dequeue(JobQueue* queue){
	if(queue == NULL || queue->size == 0){
		return NULL;
	}

	JobNode* temp = queue->head;
	Job* job = temp->job;

	queue->head = queue->head->next;
	queue->size--;

	if(queue->size == 0){
		queue->tail = NULL;
	}

	free(temp);

	return job;
}


