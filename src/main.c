#include <stdio.h>
#include <unistd.h>
#include "job.h"
#include "job_vector.h"
#include "job_queue.h"
#include "scheduler.h"
#include "priority_scheduler.h"
#include <pthread.h>
#include "worker.h"
#include "engine.h"

int main(void){

	printf("Linux Job Engine\n");

	Engine engine;

	if(!engine_init(&engine)){
		return 1;
	}

	const char* commands[] = {
		"true",
		"false",
		"true",
		"true",
		"false",
		"true",
		"true",
		"false"
	};

	int priorities[] = {3, 9, 5, 4, 10, 1, 6, 7};

	for(size_t i = 0; i < 8; i += 1){
		engine_add_job(&engine, job_create(i, commands[i], priorities[i]));
	}

	int result = engine_run(&engine);

	engine_destroy(&engine);

	return result;
}
