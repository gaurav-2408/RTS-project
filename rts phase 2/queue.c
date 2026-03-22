#include "task.h"

void insert_job(Job **head, Job *new_job, int is_edf) {

    if (*head == NULL) {
        new_job->next = NULL;
        *head = new_job;
        return;
    }

    Job *curr = *head;
    Job *prev = NULL;

    while (curr != NULL) {

        int better;

        if (is_edf)
            better = new_job->abs_deadline < curr->abs_deadline;
        else
            better = task_set[new_job->task_id - 1].period <
                     task_set[curr->task_id - 1].period;

        if (better) break;

        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        new_job->next = *head;
        *head = new_job;
    } else {
        prev->next = new_job;
        new_job->next = curr;
    }
}