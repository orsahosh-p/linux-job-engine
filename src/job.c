#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <job.h>

int job_count = 0;

void job_print(const Job * job){
	printf("\nID: %ld\n", job->id);
	printf("Command: %s\n", job->command);
	printf("Priority: %d\n", job->priority);
	printf("State: %s\n", job_state_to_string(job->state));
	printf("Pid: %d\n", job->pid);
	printf("Exit code: %d\n\n", job->exit_code);
}

const char * job_state_to_string(JobState state){

	switch(state){
		case JOB_PENDING:
			return "PENDING";
		case JOB_RUNNING:
			return "RUNNING";
		case JOB_COMPLETED:
			return "COMPLETED";
		case JOB_FAILED:
			return "FAILED";
		default:
			return "UNKNOWN";
	}
}

bool job_can_transition(JobState current, JobState next){
	if(current == JOB_PENDING){
		if(next == JOB_RUNNING || next == JOB_FAILED){
			return 1;
		}
	}else if(current == JOB_RUNNING){
		if(next == JOB_COMPLETED || next == JOB_FAILED){
			return 1;
		}
	}

	return 0;
}

bool job_set_state(Job* job, JobState next){
	if(job == NULL){
		return 0;
	}

	if(!job_can_transition(job->state, next)){
		return 0;
	}

	job->state = next;

	return 1;
}

Job* job_create(uint64_t id, const char * command, int priority){
	if(command == NULL || strlen(command) == 0){
		return NULL;
	}

	if(priority < 0 || priority > 10){
		return NULL;
	}

	Job* job = malloc(sizeof(Job));

	if(job == NULL){
		return NULL;
	}

	size_t len = strlen(command);

	if(len == SIZE_MAX){
		free(job);
		return NULL;
	}

	char * cmd = malloc(len + 1); 

	if(cmd == NULL){
		free(job);
		return NULL;
	}

	memcpy(cmd, command, len + 1);

	job->id = id;
	job->command = cmd;
	job->priority = priority;
	job->state = JOB_PENDING;
	job->pid = -1;
	job->exit_code = 0;

	return job;
}

void job_destroy(Job* job){
	if(job == NULL){
		return;
	}

	free(job->command);

	free(job);
}

