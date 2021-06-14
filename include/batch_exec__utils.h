#include <stdio.h>

#define MPI_BATCH_BUFSIZ 2048

long count_lines(FILE *fp);

void update_progress_bar(int curr, int total, const char* desc);

char* trim_whitespace(char* str);

char* pop_line(FILE *fp, char* buf, size_t bufsz);

void run_cmd(const char* cmd, char* output, size_t output_size);
