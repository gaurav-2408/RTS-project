#include "task.h"
#include "actual.h"
#include "parameters.h"

int main(int argc, char *argv[]) {

    FILE *fp = fopen(argv[1], "r");
    getdata(fp);
    fclose(fp);

    load_actual("actual.txt");

    int hp = compute_hyperperiod();

    edf_scheduler(hp);

    printParameters(hp);

    return 0;
}