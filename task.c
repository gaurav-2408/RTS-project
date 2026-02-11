#include "task.h"

Task task_set[MAX_TASKS];
Job job_list[MAX_JOBS];

int task_count = 0;
int job_count = 0;

static int gcd(int a, int b) {
    return (b == 0) ? a : gcd(b, a % b);
}

static int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

void getdata(FILE *fp) {
    fscanf(fp, "%d", &task_count);

    for (int i = 0; i < task_count; i++) {
        fscanf(fp, "%d %d %d %d",
               &task_set[i].phase,
               &task_set[i].period,
               &task_set[i].wcet,
               &task_set[i].deadline);
    }
}

int compute_hyperperiod() {
    int hp = task_set[0].period;
    for (int i = 1; i < task_count; i++)
        hp = lcm(hp, task_set[i].period);
    return hp;
}

void generate_jobs(int hyperperiod) {
    job_count = 0;

    for (int i = 0; i < task_count; i++) {
        int jid = 1;
        for (int t = task_set[i].phase; t < hyperperiod; t += task_set[i].period) {
            job_list[job_count++] = (Job){
                .task_id = i + 1,
                .job_id = jid++,
                .arrival = t,
                .abs_deadline = t + task_set[i].deadline,
                .period = task_set[i].period,
                .remaining_time = task_set[i].wcet,
                .last_exec_time = -1
            };
        }
    }
}

