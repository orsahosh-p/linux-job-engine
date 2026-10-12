#include <stdio.h>
#include <assert.h>
#include <job.h>

int main(void){

Job job = {
	1,
	"test",
	10,
	JOB_PENDING,
	-1,
	0
};

assert(job_can_transition(JOB_PENDING, JOB_RUNNING) == 1);
assert(job_can_transition(JOB_PENDING, JOB_FAILED) == 1);
assert(job_can_transition(JOB_RUNNING, JOB_COMPLETED) == 1);
assert(job_can_transition(JOB_RUNNING, JOB_FAILED) == 1);
assert(job_can_transition(JOB_PENDING, JOB_COMPLETED) == 0);
assert(job_can_transition(JOB_COMPLETED, JOB_RUNNING) == 0);
assert(job_can_transition(JOB_FAILED, JOB_PENDING) == 0);
assert(job_can_transition(JOB_PENDING, JOB_PENDING) == 0);
assert(job_can_transition(23, JOB_RUNNING) == 0);
assert(job_set_state(NULL, JOB_RUNNING) == 0);
assert(job_set_state(&job, JOB_RUNNING) == 1);
assert(job.state == JOB_RUNNING);
assert(job_set_state(&job, JOB_PENDING) == 0);
assert(job.state != JOB_PENDING);
	return 0;
}
