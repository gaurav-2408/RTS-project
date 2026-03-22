#include "task.h"

Task *task_set;
int task_count = 0;

ExecLog logs[MAX_LOG];
int log_count = 0;

JobStat job_stats[MAX_TASKS][MAX_JOBS_PER_TASK];
int job_count_task[MAX_TASKS] = {0};

static int gcd(int a, int b) {
    return (b == 0) ? a : gcd(b, a % b);
}

static int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

void getdata(FILE *fp) {
    fscanf(fp, "%d", &task_count);

    task_set = malloc(task_count * sizeof(Task));

    for (int i = 0; i < task_count; i++) {
        fscanf(fp, "%d %d %d %d",
               &task_set[i].phase,
               &task_set[i].period,
               &task_set[i].wcet,
               &task_set[i].deadline);

        task_set[i].phase *= 10;
        task_set[i].period *= 10;
        task_set[i].wcet *= 10;
        task_set[i].deadline *= 10;

        task_set[i].next_release = task_set[i].phase;
    }
}

double compute_utilization() {
    double U = 0;
    for (int i = 0; i < task_count; i++)
        U += (double)task_set[i].wcet / task_set[i].period;
    return U;
}

int compute_hyperperiod() {
    int hp = task_set[0].period;
    for (int i = 1; i < task_count; i++)
        hp = lcm(hp, task_set[i].period);
    return hp;
}

void free_queue(Job *rq) {
    while (rq) {
        Job *t = rq;
        rq = rq->next;
        free(t);
    }
}