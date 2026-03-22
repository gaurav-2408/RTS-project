#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 50
#define MAX_LOG 20000
#define MAX_JOBS_PER_TASK 2000

typedef struct {
    int phase;
    int period;
    int wcet;
    int deadline;
    int next_release;
} Task;

typedef struct Job {
    int task_id;
    int job_id;
    int arrival;
    int abs_deadline;
    int remaining_time;
    struct Job *next;
} Job;

/* Execution Log */
typedef struct {
    int start;
    int end;
    int task_id;
    int job_id;
} ExecLog;

/* Job Stats */
typedef struct {
    int arrival;
    int first_start;
    int finish;
    int execution;
    int deadline;
} JobStat;

extern Task *task_set;
extern int task_count;

extern ExecLog logs[MAX_LOG];
extern int log_count;

extern JobStat job_stats[MAX_TASKS][MAX_JOBS_PER_TASK];
extern int job_count_task[MAX_TASKS];

extern int voluntary_cs;
extern int involuntary_cs;
extern int preemptions_count;

void getdata(FILE *fp);
double compute_utilization();
int compute_hyperperiod();

void insert_job(Job **head, Job *new_job, int is_edf);
void free_queue(Job *rq);

void edf_scheduler(int hp);
void rm_scheduler(int hp);

#endif