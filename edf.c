#include "task.h"
#include <limits.h>

static int select_edf(int time, int last) {
    int chosen = -1, min_dl = INT_MAX;

    for (int i = 0; i < job_count; i++) {
        if (job_list[i].arrival <= time &&
            job_list[i].remaining_time > 0) {

            if (job_list[i].abs_deadline < min_dl ||
               (job_list[i].abs_deadline == min_dl &&
                job_list[i].last_exec_time >
                job_list[chosen].last_exec_time)) {
                min_dl = job_list[i].abs_deadline;
                chosen = i;
            }
        }
    }

    if (last != -1 &&
        job_list[last].remaining_time > 0 &&
        job_list[last].abs_deadline == min_dl)
        return last;

    return chosen;
}

void edf_scheduler(int hyperperiod) {
    int last = -1, start = 0;

    printf("\n===== EDF SCHEDULE =====\n");

    for (int t = 0; t < hyperperiod; t++) {
        int curr = select_edf(t, last);

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

