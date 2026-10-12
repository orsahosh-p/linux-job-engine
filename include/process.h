#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

pid_t process_start(const char *program, char *const argv[]);
int process_check(pid_t pid, int * exit_code);
int process_terminate(pid_t pid);

#endif
