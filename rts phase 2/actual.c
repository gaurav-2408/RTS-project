#include "actual.h"

int actual_exec[MAX_TASKS][MAX_JOBS_PER_TASK];

void load_actual(char *filename) {

    FILE *fp = fopen(filename, "r");

    for (int i = 0; i < task_count; i++) {
        for (int j = 0; j < MAX_JOBS_PER_TASK; j++) {
            if (fscanf(fp, "%d", &actual_exec[i][j]) != 1)
                break;
        }
    }

    fclose(fp);
}