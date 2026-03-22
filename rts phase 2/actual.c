#include "actual.h"

int actual_exec[MAX_TASKS][MAX_JOBS_PER_TASK];

void load_actual(char *filename) {

    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char line[1024];
    for (int i = 0; i < task_count; i++) {
        if (!fgets(line, sizeof(line), fp))
            break;

        char *p = line;
        int j = 0;

        while (j < MAX_JOBS_PER_TASK) {
            float temp;
            int read_chars;

            if (sscanf(p, "%f%n", &temp, &read_chars) != 1)
                break;

            actual_exec[i][j++] = (int)(temp * 10 + 0.5);
            p += read_chars;
        }

        for (; j < MAX_JOBS_PER_TASK; j++)
            actual_exec[i][j] = 0;
    }

    fclose(fp);
}
