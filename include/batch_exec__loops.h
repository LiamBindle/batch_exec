#include <stdio.h>

int send_string_to_worker(const char* curr_line, int dest, int *term_cnt);

void root_proc_loop(const int NUM_PROCS, FILE* batch_file, FILE* output);

void worker_proc_loop(const int PROC_ID, const int NUM_PROCS, const char* cmd_prefix, int dry_run);
