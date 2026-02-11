#include "task.h"
#include <limits.h>

static int select_rm(int time, int last) {
    int chosen = -1, min_p = INT_MAX;

    for (int i = 0; i < job_count; i++) {
        if (job_list[i].arrival <= time &&
            job_list[i].remaining_time > 0) {

            if (job_list[i].period < min_p ||
               (job_list[i].period == min_p &&
                job_list[i].last_exec_time >
                job_list[chosen].last_exec_time)) {
                min_p = job_list[i].period;
                chosen = i;
            }
        }
    }

    if (last != -1 &&
        job_list[last].remaining_time > 0 &&
        job_list[last].period == min_p)
        return last;

    return chosen;
}

void rm_scheduler(int hyperperiod) {
    int last = -1, start = 0;

    printf("\n===== RM SCHEDULE =====\n");

    for (int t = 0; t < hyperperiod; t++) {
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

    if (last != -1)
        printf("[%d] [%d] [J%d] [T%d]\n",
               start, hyperperiod,
               job_list[last].job_id,
               job_list[last].task_id);
}

