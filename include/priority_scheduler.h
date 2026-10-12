#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "scheduler.h"
#include "job_heap.h"

int priority_scheduler_run(Scheduler* scheduler, JobHeap* heap);

#endif
