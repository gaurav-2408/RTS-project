#ifndef TASK_H
#define TASK_H

#include <stdio.h>

#define MAX_TASKS 50
#define MAX_JOBS  500

typedef struct {
    int phase;
    int period;
    int wcet;
    int deadline;
} Task;

typedef struct {
    int task_id;
    int job_id;
    int arrival;
    int abs_deadline;
    int period;
    int remaining_time;
    int last_exec_time;
} Job;

/* Global ADT storage */
extern Task task_set[MAX_TASKS];
extern Job job_list[MAX_JOBS];
extern int task_count;
extern int job_count;

/* Core ADT functions */
void getdata(FILE *fp);
int compute_hyperperiod();
void generate_jobs(int hyperperiod);

/* Scheduling functions */
void edf_scheduler(int hyperperiod);
void rm_scheduler(int hyperperiod);

#endif

