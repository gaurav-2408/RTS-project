#ifndef ACTUAL_H
#define ACTUAL_H

#include "task.h"

extern int actual_exec[MAX_TASKS][MAX_JOBS_PER_TASK];

void load_actual(char *filename);

#endif