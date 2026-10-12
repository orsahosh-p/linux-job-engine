#include <assert.h>
#include <job.h>
#include <string.h>

int main(void){

Job* job = job_create(101, "/bin/ls", 10);

assert(job->id == 101);
assert(strcmp(job->command, "/bin/ls") == 0);
assert(job->priority == 10);
assert(job->state == JOB_PENDING);
assert(job->pid == -1);
assert(job->exit_code == 0);

Job* job_command = job_create(102, "", 10);
assert(job_command == NULL);

Job* job_priority = job_create(102, "cmd", 11);
assert(job_priority == NULL);

job_destroy(NULL);

job_destroy(job);
job_destroy(job_command);
job_destroy(job_priority);

	return 0;
}
