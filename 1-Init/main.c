#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/resource.h>       //getrlimit
#include <unistd.h>     // getppid
#include <stdlib.h>     // exit

int main(void) {

    struct rlimit flim;

    if (getppid() != 1) {
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
    }

    if (fork() != 0) {
        exit(0);
    }

    setsid();

    getrlimit(RLIMIT_NOFILE, &flim);

    int fd;
    for (fd = 0; fd < flim.rlim_max; fd++) {
        close(fd);
    }

    chdir("/");

    openlog("Daemon Init", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "Init start");
    closelog();

    return 0;
}

