#include "task.h"

int main(int argc, char *argv[]) {
    FILE *fp;
    int hp;

    if (argc != 2) {
        printf("Usage: %s input.txt\n", argv[0]);
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (!fp) {
        printf("File error\n");
        return 1;
    }

    getdata(fp);
    fclose(fp);

    hp = compute_hyperperiod();

    generate_jobs(hp);
    edf_scheduler(hp);

    generate_jobs(hp);
    rm_scheduler(hp);

    return 0;
}

