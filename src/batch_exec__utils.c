#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "batch_exec__utils.h"

long int count_lines(FILE *fp) {
    int rv;
    long int num_lines = 0;

    rewind(fp);

    if (fp == NULL) {
        return -1;
    }

    while (EOF != fscanf(fp, "%*s")) {  /* scan to next non-whitespace string */
        ++num_lines;
        fscanf(fp, "%*[^\n]");          /* scan to next new line */
    }

    return num_lines;
}

void update_progress_bar(int curr, int total, const char* desc) {
    int width=60;
    int progress = curr*width/total;
    int pos;

    printf("\r [");
    for(pos = 0; pos < progress && pos < width; ++pos) {
        printf("=");
    }
    for(; pos < width; ++pos) {
        printf(" ");
    }
    printf("] %i/%i %s", curr, total, desc);
    if (curr == total) {
        printf("\n");
    }
    fflush(stdout);
}

char* trim_whitespace(char* str) {
    char* start = str;
    char* end = str + strlen(str) - 1;
    /* Move start to first non-whitespace character */
    while(*start != '\0' && isspace(*start)) {
        start++;
    }
    if (*start == '\0') {
        return NULL; /* all whitespace */
    }
    /* Move end to last non-whitespace character */
    while(*end != '\0' && isspace(*end)) {
        end--;
    }
    /* Add nullchar at end[1] */
    end[1] = '\0';
    return start;
}

char* pop_line(FILE *fp, char* buf, size_t bufsz) {
    char* line;
    while (NULL != fgets(buf, bufsz, fp)) {
        line = trim_whitespace(buf);
        if (line != NULL){
            return line;
        }
    }
    return NULL; /* EOF */
}

void run_cmd(const char* cmd, char* output, size_t output_size) {
    size_t line_len;
    const char* start = output;

    FILE *fp = popen(cmd, "r");
    while(fgets(output, output_size, fp) != NULL) {
        line_len = strlen(output);
        output_size -= line_len;
        output += line_len;
    }

    if(isspace(*(output-1))) {
        *(output-1) = '\0';
    }
    pclose(fp);
}
