#include <stdio.h>
#include <unistd.h>

void fprint(char *message) {
    FILE* log = fopen ("/home/ksenia/OS/Log.txt", "a+");
    fprintf(log, "%d : ", getpid());
    fprintf(log, "%s\n", message);
    fflush(log);
    fclose(log);
    return;
}
