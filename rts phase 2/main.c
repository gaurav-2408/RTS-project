#include "task.h"
#include "actual.h"
#include "parameters.h"
#include <string.h>

int main(int argc, char *argv[]) {

    FILE *fp = fopen(argv[1], "r");
    getdata(fp);
    fclose(fp);

    load_actual("actual.txt");

    int hp = compute_hyperperiod();

    // EDF Scheduler
    edf_scheduler(hp);
    printParameters("EDF", hp, 1);

    // Reset for RM
    log_count = 0;
    memset(job_stats, 0, sizeof(job_stats));
    memset(job_count_task, 0, sizeof(job_count_task));
    for(int i = 0; i < task_count; i++) {
        task_set[i].next_release = task_set[i].phase;
    }

    // RM Scheduler
    rm_scheduler(hp);
    printParameters("RM", hp, 0);

    return 0;
}