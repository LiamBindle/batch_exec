
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "batch_exec__utils.h"
#include "batch_exec__loops.h"

#include <stdio.h>
#include <mpi.h>

#include "batch_exec__loops.h"

int main(int argc, char **argv) {
    char tempbuf[100];
    int dry_run = 0;
    int PROC_ID, NUM_PROCS;
    int ierr;
    const char* answer = "1\n2\n3\n4\n";
    FILE *output;
    memset(tempbuf, '\0', sizeof(tempbuf));

    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &PROC_ID);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCS);

    if (PROC_ID == 0) {
        FILE *input = fopen("test_batch_3.txt", "r");
        output = fopen("test_exec_output.txt", "w+");
        root_proc_loop(NUM_PROCS, input, output);
    } else {
        worker_proc_loop(PROC_ID, NUM_PROCS, tempbuf, dry_run);
    }

    ierr = MPI_Finalize();

    if (PROC_ID == 0) {
        fseek(output, 0, SEEK_END);
        long fsize = ftell(output);

        rewind(output);
        fread(tempbuf, 1, fsize, output);
        fclose(output);
        tempbuf[fsize] = '\0';

        assert(strcmp(tempbuf, answer) == 0);
    }

    return 0;
}
