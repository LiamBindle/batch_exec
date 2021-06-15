#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "batch_exec__utils.h"
#include "batch_exec__loops.h"


int send_string_to_worker(const char* curr_line, int dest, int *term_cnt) {
    if (curr_line != NULL) {
        /* Send string to worker */
        return MPI_Send(curr_line, strlen(curr_line)+1, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
    } else {
        /* Send terminate message (empty string) to worker and increment counter */
        (*term_cnt)++;
        return MPI_Send("", 1, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
    }
}


void root_proc_loop(const int NUM_PROCS, FILE* batch_file, FILE* output) {
    MPI_Status status;
    char iobuf[MPI_BATCH_BUFSIZ];
    int ierr, temp;
    long output_size;
    char* curr_line;
    int term_count = 0;
    rewind(batch_file);

    /* Send out first batch of jobs */
    for(temp = 1; temp < NUM_PROCS; ++temp) {
        curr_line = pop_line(batch_file, iobuf, sizeof(iobuf));
        ierr = send_string_to_worker(curr_line, temp, &term_count);
        if(ierr != MPI_SUCCESS) {
            MPI_Error_string(ierr, iobuf, &temp);
            iobuf[temp] = '\0';
            fprintf(stderr, "%s\n", iobuf);
        }
    }

    while(term_count < NUM_PROCS - 1) {
        /* Receive the next response */
        MPI_Recv(iobuf, sizeof(iobuf), MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        fprintf(output, "%s\n", iobuf);

        /* Dispatch next */
        curr_line = pop_line(batch_file, iobuf, sizeof(iobuf));
        ierr = send_string_to_worker(curr_line, status.MPI_SOURCE, &term_count);
        if(ierr != MPI_SUCCESS) {
            MPI_Error_string(ierr, iobuf, &temp);
            iobuf[temp] = '\0';
            fprintf(stderr, "%s\n", iobuf);
        }
    }
}

void worker_proc_loop(const int PROC_ID, const int NUM_PROCS, const char* cmd_prefix, int dry_run) {
    char recvbuf[MPI_BATCH_BUFSIZ];
    char tempbuf[MPI_BATCH_BUFSIZ];
    int ierr, temp;
    long output_size;

    while(1) {
        ierr = MPI_Recv(recvbuf, sizeof(recvbuf), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /* Handle errors */
        if (ierr != MPI_SUCCESS) {
            MPI_Error_string(ierr, recvbuf, &temp);
            recvbuf[temp] = '\0';
            fprintf(stderr, "%s\n", recvbuf);
            break;
        }

        /* Empty string means terminate */
        if(strlen(recvbuf) == 0) {
            break;
        }

        /* Process current */
        sprintf(tempbuf, "%s%s", cmd_prefix, recvbuf);
        if (!dry_run) {
            run_cmd(tempbuf, tempbuf, sizeof(tempbuf));
        }

        MPI_Send(tempbuf, strlen(tempbuf)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
}
