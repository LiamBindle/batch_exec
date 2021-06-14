
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "batch_exec__utils.h"

int main(int argc, char **argv) {
    char buf[MPI_BATCH_BUFSIZ];
    char *temp_c;
    FILE *file1 = fopen("testfile1.txt", "r");
    FILE *file2 = fopen("testfile2.txt", "r");

    /* test count_lines */
    assert(count_lines(file1) == 5);
    assert(count_lines(file2) == 4);

    /* test trimming whitespace */
    strcpy(buf, "    this is a test     \n\t");
    temp_c = trim_whitespace(buf);
    assert(strcmp(temp_c, "this is a test") == 0);

    /* test pop_line */
    rewind(file2);
    pop_line(file2, buf, sizeof(buf));
    assert(strcmp(buf, "this is a") == 0);
    pop_line(file2, buf, sizeof(buf));
    assert(strcmp(buf, "test   to sees") == 0);
    pop_line(file2, buf, sizeof(buf));
    assert(strcmp(buf, "how many lines") == 0);
    pop_line(file2, buf, sizeof(buf));
    assert(strcmp(buf, "the program counts") == 0);
    assert(pop_line(file2, buf, sizeof(buf)) == NULL); /* EOF */

    /* test run_cmd */
    run_cmd("echo this is an echo test", buf, sizeof(buf));
    assert(strcmp(buf, "this is an echo test") == 0);

    run_cmd("echo this is an echo test 2 && echo this is a second line", buf, sizeof(buf));
    printf("%s\n\n", buf);
    fflush(stdout);
    assert(strcmp(buf, "this is an echo test 2\nthis is a second line") == 0);

    return 0;
}
