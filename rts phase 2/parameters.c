#include "parameters.h"
#include <stdio.h>

void printParameters(char *name, int hp, int is_first) {

    FILE *fp = fopen("metrics.txt", is_first ? "w" : "a");

    fprintf(fp, "\n%s Scheduler Results:\n", name);

    int decision_points = log_count;
    int context_switch = log_count - 1;
    int preemptions = 0;
    int cache_points = 0;

    for (int i = 1; i < log_count; i++) {

        if (logs[i].task_id != logs[i-1].task_id) {

            int prev_t = logs[i-1].task_id - 1;
            int prev_j = logs[i-1].job_id - 1;

            if (job_stats[prev_t][prev_j].finish > logs[i].start)
                preemptions++;

            if (logs[i].task_id != logs[i-1].task_id)
                cache_points++;
        }
    }

    int busy = 0;
    for (int i = 0; i < log_count; i++)
        busy += logs[i].end - logs[i].start;

    double cpu_util = (double)busy / hp;

    fprintf(fp, "Decision Points: %d\n", decision_points);
    fprintf(fp, "Context Switches: %d\n", context_switch);
    fprintf(fp, "Preemptions: %d\n", preemptions);
    fprintf(fp, "Cache Impact: %d\n", cache_points);
    fprintf(fp, "CPU Utilization: %.3f\n\n", cpu_util);

    for (int t = 0; t < task_count; t++) {

        double min_rt = 1e9, max_rt = 0, sum_rt = 0;
        double min_wt = 1e9, max_wt = 0, sum_wt = 0;
        double min_lat = 1e9, max_lat = 0, sum_lat = 0;
        double min_late = 1e9, max_late = 0, sum_late = 0;

        int count = job_count_task[t];

        for (int j = 0; j < count; j++) {

            int arrival = job_stats[t][j].arrival;
            int finish = job_stats[t][j].finish;
            int start = job_stats[t][j].first_start;
            int exec = job_stats[t][j].execution;
            int deadline = job_stats[t][j].deadline;

            int response = finish - arrival;
            int waiting = response - exec;
            int latency = finish - start;
            int lateness = finish - deadline;

            if (response < min_rt) min_rt = response;
            if (response > max_rt) max_rt = response;
            sum_rt += response;

            if (waiting < min_wt) min_wt = waiting;
            if (waiting > max_wt) max_wt = waiting;
            sum_wt += waiting;

            if (latency < min_lat) min_lat = latency;
            if (latency > max_lat) max_lat = latency;
            sum_lat += latency;

            if (lateness < min_late) min_late = lateness;
            if (lateness > max_late) max_late = lateness;
            sum_late += lateness;
        }

        fprintf(fp, "Task %d:\n", t+1);

        fprintf(fp, "Response (min/avg/max): %.2f %.2f %.2f\n",
                min_rt, sum_rt/count, max_rt);

        fprintf(fp, "Waiting (min/avg/max): %.2f %.2f %.2f\n",
                min_wt, sum_wt/count, max_wt);

        fprintf(fp, "Latency (min/avg/max): %.2f %.2f %.2f\n",
                min_lat, sum_lat/count, max_lat);

        fprintf(fp, "Lateness (min/avg/max): %.2f %.2f %.2f\n\n",
                min_late, sum_late/count, max_late);
    }

    fprintf(fp, "Schedule:\n");
    for(int i = 0; i < log_count; i++){
        fprintf(fp, "Task %d Job %d: %d - %d\n", logs[i].task_id, logs[i].job_id, logs[i].start, logs[i].end);
    }
    fprintf(fp, "\n");

    fclose(fp);
}