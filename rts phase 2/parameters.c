#include "parameters.h"
#include <stdio.h>
#include <limits.h>

void printParameters(char *name, int hp, int is_first) {

    FILE *fp = fopen("metrics.txt", is_first ? "w" : "a");

    fprintf(fp, "\n%s Scheduler Results:\n", name);

    int decision_points = log_count;
    int context_switch = voluntary_cs + involuntary_cs;
    int cache_points = 0;

    for (int i = 1; i < log_count; i++) {
        if (logs[i].task_id != logs[i-1].task_id)
            cache_points++;
    }

    // preemptions_count comes from scheduler


    int busy = 0;
    for (int i = 0; i < log_count; i++) {
        if (logs[i].task_id != 0)  // ignore idle entries in CPU busy time
            busy += logs[i].end - logs[i].start;
    }

    double cpu_util = (double)busy / hp;

    fprintf(fp, "Decision Points: %d\n", decision_points);
    fprintf(fp, "Context Switches: %d (voluntary=%d involuntary=%d)\n", context_switch, voluntary_cs, involuntary_cs);
    fprintf(fp, "Preemptions: %d\n", preemptions_count);
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

            double resp_d = response / 10.0;
            double wait_d = waiting / 10.0;
            double lat_d = latency / 10.0;
            double late_d = lateness / 10.0;

            if (resp_d < min_rt) min_rt = resp_d;
            if (resp_d > max_rt) max_rt = resp_d;
            sum_rt += resp_d;

            if (wait_d < min_wt) min_wt = wait_d;
            if (wait_d > max_wt) max_wt = wait_d;
            sum_wt += wait_d;

            if (lat_d < min_lat) min_lat = lat_d;
            if (lat_d > max_lat) max_lat = lat_d;
            sum_lat += lat_d;

            if (late_d < min_late) min_late = late_d;
            if (late_d > max_late) max_late = late_d;
            sum_late += late_d;
        }

        double abs_arr_jitter = 0.0;
        double rel_arr_jitter = 0.0;
        if (count > 1) {
            int min_inter = INT_MAX;
            int max_inter = 0;
            for (int j2 = 1; j2 < count; j2++) {
                int delta = job_stats[t][j2].arrival - job_stats[t][j2-1].arrival;
                if (delta < min_inter) min_inter = delta;
                if (delta > max_inter) max_inter = delta;
            }
            abs_arr_jitter = (max_inter - min_inter) / 10.0;
            rel_arr_jitter = (task_set[t].period > 0) ? abs_arr_jitter / (task_set[t].period / 10.0) : 0.0;
        }

        double abs_resp_jitter = max_rt - min_rt;
        double avg_rt = (count > 0) ? sum_rt / count : 0.0;
        double rel_resp_jitter = (avg_rt > 0.0) ? abs_resp_jitter / avg_rt : 0.0;

        fprintf(fp, "Task %d:\n", t+1);

        fprintf(fp, "Response (min/avg/max): %.2f %.2f %.2f\n",
                min_rt, sum_rt/count, max_rt);

        fprintf(fp, "Waiting (min/avg/max): %.2f %.2f %.2f\n",
                min_wt, sum_wt/count, max_wt);

        fprintf(fp, "Latency (min/avg/max): %.2f %.2f %.2f\n",
                min_lat, sum_lat/count, max_lat);

        fprintf(fp, "Lateness (min/avg/max): %.2f %.2f %.2f\n",
                min_late, sum_late/count, max_late);

        fprintf(fp, "Arrival Jitter (abs/rel): %.2f %.2f\n", abs_arr_jitter, rel_arr_jitter);
        fprintf(fp, "Response Jitter (abs/rel): %.2f %.2f\n\n", abs_resp_jitter, rel_resp_jitter);
    }

    fprintf(fp, "Schedule:\n");
    for(int i = 0; i < log_count; i++){
        if (logs[i].task_id == 0) {
            fprintf(fp, "Idle: %.1f - %.1f\n", logs[i].start / 10.0, logs[i].end / 10.0);
        } else {
            fprintf(fp, "Task %d Job %d: %.1f - %.1f\n", logs[i].task_id, logs[i].job_id, logs[i].start / 10.0, logs[i].end / 10.0);
        }
    }
    fprintf(fp, "\n");

    fclose(fp);
}