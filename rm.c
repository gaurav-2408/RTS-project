#include "task.h"
#include <limits.h>
#include <stdio.h>

static int select_rm(int time, int last) {
    int chosen = -1;
    int min_period = INT_MAX;

    for (int i = 0; i < job_count; i++) {

        if (job_list[i].arrival <= time &&
            job_list[i].remaining_time > 0) {

            if (chosen == -1) {
                chosen = i;
                min_period = job_list[i].period;
            }
            else if (job_list[i].period < min_period) {
                chosen = i;
                min_period = job_list[i].period;
            }
            else if (job_list[i].period == min_period) {
                // tie-breaking: most recently executed
                if (job_list[i].last_exec_time >
                    job_list[chosen].last_exec_time) {
                    chosen = i;
                }
            }
        }
    }

    // If last job has same highest priority, continue it
    if (last != -1 &&
        job_list[last].remaining_time > 0 &&
        job_list[last].arrival <= time &&
        job_list[last].period == min_period) {
        return last;
    }

    return chosen;
}

void rm_scheduler(int hyperperiod) {
    int last = -1;
    int start = 0;

    printf("\n===== RM SCHEDULE =====\n");

    for (int t = 0; t < hyperperiod; t++) {

        // Deadline miss check
        for (int i = 0; i < job_count; i++) {
            if (job_list[i].remaining_time > 0 &&
                t >= job_list[i].abs_deadline) {
                printf("Deadline Miss at time %d : J%d (T%d)\n",
                       t,
                       job_list[i].job_id,
                       job_list[i].task_id);
                return;
            }
        }

        int curr = select_rm(t, last);

        if (curr != last) {
            if (last != -1) {
                printf("[%d] [%d] [J%d] [T%d]\n",
                       start, t,
                       job_list[last].job_id,
                       job_list[last].task_id);
            }
            start = t;
        }

        if (curr != -1) {
            job_list[curr].remaining_time--;
            job_list[curr].last_exec_time = t;
        }

        last = curr;
    }

    if (last != -1) {
        printf("[%d] [%d] [J%d] [T%d]\n",
               start, hyperperiod,
               job_list[last].job_id,
               job_list[last].task_id);
    }
}
