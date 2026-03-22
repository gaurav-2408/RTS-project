#include "task.h"
#include "actual.h"

void edf_scheduler(int hp) {

    Job *rq = NULL;
    int time = 0;
    int job_id[MAX_TASKS] = {0};

    while (time < hp) {

        /* ARRIVALS */
        for (int i = 0; i < task_count; i++) {

            if (time == task_set[i].next_release) {

                Job *j = malloc(sizeof(Job));

                j->task_id = i + 1;
                j->job_id = ++job_id[i];
                j->arrival = time;
                j->abs_deadline = time + task_set[i].deadline;

                /* ACTUAL EXECUTION */
                j->remaining_time = actual_exec[i][job_id[i]-1];

                j->next = NULL;

                insert_job(&rq, j, 1);

                /* JOB STATS INIT */
                int tid = i;
                int jid = job_id[i] - 1;

                job_stats[tid][jid].arrival = time;
                job_stats[tid][jid].execution = j->remaining_time;
                job_stats[tid][jid].deadline = j->abs_deadline;
                job_stats[tid][jid].first_start = -1;

                job_count_task[i]++;

                task_set[i].next_release += task_set[i].period;
            }
        }

        if (!rq) {
            time++;
            continue;
        }

        Job *curr = rq;
        rq = rq->next;

        int start = time;

        int tid = curr->task_id - 1;
        int jid = curr->job_id - 1;

        /* FIRST START */
        if (job_stats[tid][jid].first_start == -1)
            job_stats[tid][jid].first_start = time;

        while (1) {

            int next_arrival = 1e9;

            for (int i = 0; i < task_count; i++) {
                if (task_set[i].next_release > time &&
                    task_set[i].next_release < next_arrival)
                    next_arrival = task_set[i].next_release;
            }

            int completion = time + curr->remaining_time;
            int next_time = (next_arrival < completion) ? next_arrival : completion;

            curr->remaining_time -= (next_time - time);
            time = next_time;

            /* ARRIVALS AGAIN */
            for (int i = 0; i < task_count; i++) {

                if (time == task_set[i].next_release) {

                    Job *j = malloc(sizeof(Job));

                    j->task_id = i + 1;
                    j->job_id = ++job_id[i];
                    j->arrival = time;
                    j->abs_deadline = time + task_set[i].deadline;

                    j->remaining_time = actual_exec[i][job_id[i]-1];

                    j->next = NULL;

                    insert_job(&rq, j, 1);

                    int tid2 = i;
                    int jid2 = job_id[i] - 1;

                    job_stats[tid2][jid2].arrival = time;
                    job_stats[tid2][jid2].execution = j->remaining_time;
                    job_stats[tid2][jid2].deadline = j->abs_deadline;
                    job_stats[tid2][jid2].first_start = -1;

                    job_count_task[i]++;

                    task_set[i].next_release += task_set[i].period;
                }
            }

            /* COMPLETION */
            if (curr->remaining_time == 0) {

                job_stats[tid][jid].finish = time;

                logs[log_count++] =
                    (ExecLog){start, time, curr->task_id, curr->job_id};

                if (rq != NULL)
                    voluntary_cs++;

                free(curr);
                break;
            }

            /* REINSERT */
            insert_job(&rq, curr, 1);

            Job *next = rq;
            rq = rq->next;

            if (next != curr) {

                involuntary_cs++;
                preemptions_count++;
                logs[log_count++] =
                    (ExecLog){start, time, curr->task_id, curr->job_id};

                curr = next;
                start = time;

                tid = curr->task_id - 1;
                jid = curr->job_id - 1;

                if (job_stats[tid][jid].first_start == -1)
                    job_stats[tid][jid].first_start = time;
            }
        }
    }

    free_queue(rq);
}