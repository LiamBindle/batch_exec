#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>

#include "batch_exec__utils.h"
#include "batch_exec__loops.h"


int main(int argc, char **argv) {
    int ierr;
    int i;
    int opt;
    int dry_run = 0;
    FILE *input = NULL;
    FILE *output = NULL;

    char tempbuf[MPI_BATCH_BUFSIZ];
    char cmd_prefix_buf[MPI_BATCH_BUFSIZ];
    memset(cmd_prefix_buf, '\0', sizeof(cmd_prefix_buf));
    char* cmd_prefix = cmd_prefix_buf;

    int PROC_ID, NUM_PROCS;
    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &PROC_ID);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCS);

    while((opt = getopt(argc, argv, "dhi:o:a:")) != -1) { 
        switch(opt) { 
            case 'd':
                cmd_prefix += sprintf(cmd_prefix, "(");
                gethostname(tempbuf, sizeof(tempbuf));
                cmd_prefix += sprintf(cmd_prefix, "%s:%5d) ", tempbuf, (int)getpid());
                dry_run = 1;
                break;
            case 'i': 
                if (PROC_ID == 0) {
                    input = fopen(optarg, "r");
                    if (input == NULL) {
                        fprintf(stderr, "error: invalid input file \"%s\"\n", optarg);
                        exit(1);
                    }
                }
                break; 
            case 'o': 
                if (PROC_ID == 0) {
                    output = fopen(optarg, "w");
                    if (output == NULL) {
                        fprintf(stderr, "error: invalid output file \"%s\"\n", optarg);
                        exit(1);
                    }
                }
                break; 
            case 'a': 
                if (PROC_ID == 0) {
                    output = fopen(optarg, "a");
                    if (output == NULL) {
                        fprintf(stderr, "error: invalid output file \"%s\"\n", optarg);
                        exit(1);
                    }
                }
                break; 
            case 'h': 
                printf(
                    "Usage:     batch_exec [options] [command prefix...]\n"
                    "\n"
                    "Options:\n"
                    "   -d              execute dry run of batch commands\n"
                    "   -i              input file with batch commands (default is stdin)\n"
                    "   -o              output file for batch commands (default is stdout)\n"
                    "   -a              output file (in appended mode) for batch commands\n"
                    "   -h              print info and exit\n"
                );
                exit(1);
                break; 
            case '?': 
                printf("unknown option: %c\n", optopt);
                break; 
        } 
    }

    for (opt = optind; opt < argc; ++opt) {
        cmd_prefix += sprintf(cmd_prefix, "%s ", argv[opt]);
    }

    if (PROC_ID == 0) {
        if (input == NULL) {
            input = stdin;
        }
        if (output == NULL) {
             output = stdout;
        }
        root_proc_loop(NUM_PROCS, input, output);
    } else {
        worker_proc_loop(PROC_ID, NUM_PROCS, cmd_prefix_buf, dry_run);
    }

    ierr = MPI_Finalize();

    if (PROC_ID == 0) {
        fclose(input);
        fclose(output);
    }
    return 0;
}
