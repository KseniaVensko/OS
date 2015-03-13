#include <unistd.h>     // sleep()
#include <stdlib.h>     //strtol
#include <log.h>
#include <syslog.h>

int main(int argc,  char** argv) {
    openlog("Child Init", LOG_PID | LOG_CONS, LOG_USER);
    if (argc > 2 || argc < 2) {
        fprint("Too few arguments");
        return 1;
    }
    char *end;
    long int i = strtol(argv[1], &end, 10);
    i = 0 - i;
    int j;
    for (j = 0; j < i; ++j) {
        fprint("I`m working");
        sleep(5);
    }
    closelog();
    return 0;
}
