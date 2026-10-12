#ifndef JOB_H
#define JOB_H

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

extern int job_count;

typedef enum{
	JOB_PENDING,
	JOB_RUNNING,
	JOB_COMPLETED,
	JOB_FAILED
} JobState;

typedef struct{
	uint64_t id;
	char * command;
	int priority;
	JobState state;
	pid_t pid;
	int exit_code;
} Job;

void job_print(const Job * job);
const char * job_state_to_string(JobState state);
bool job_can_transition(JobState current, JobState next);
bool job_set_state(Job* job, JobState next);

Job* job_create(uint64_t id, const char * command, int priority);
void job_destroy(Job* job);

#endif
