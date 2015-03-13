#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

void fprint(char *message) {

    syslog(LOG_INFO, message);

    FILE* log = fopen ("/tmp/Log.txt", "a+");
    fprintf(log, "%d : ", getpid());
    fprintf(log, "%s\n", message);
    fflush(log);
    fclose(log);
    return;
}
