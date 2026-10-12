#define _POSIX_C_SOURCE 200809L

#include "process.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>

int process_terminate(pid_t pid){
	if(pid <= 0){
		return -1;
	}

	if(kill(pid, SIGTERM) == -1){
		perror("kill");
		return -1;
	}

	return 0;
}

pid_t process_start(const char *program, char *const argv[]){

	if(program == NULL || argv == NULL || argv[0] == NULL){
		return -1;
	}

	pid_t pid = fork();

	if(pid == -1){
		perror("fork");
		return -1;
	}else if(pid == 0){
		execvp(program, argv);

		perror("execvp");
		_exit(127);
	}
	return pid;

}

int process_check(pid_t pid, int *exit_code){
	if(pid <= 0){
		return -1;
	}

	int status;
	pid_t result;

	do{
		result = waitpid(pid, &status, WNOHANG);
	}while(result == -1 && errno == EINTR);

	if(result == -1){
		perror("waitpid");
		return -1;
	}

	if(result == 0){
		return 0;
	}

	if(WIFEXITED(status)){
		*exit_code = WEXITSTATUS(status);
	}else if(WIFSIGNALED(status)){
		*exit_code = 128 + WTERMSIG(status);
	}else{
		return -1;
	}

	return 1;
}
