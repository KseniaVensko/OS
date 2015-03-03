#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/resource.h>       //getrlimit
#include <unistd.h>     // getppid, readlink
#include <stdlib.h>     // exit

#include <read_files.h>
#include <log.h>
#define CONFIG "/home/ksenia/OS/build-1-Init/1-Init.conf"


int main(int argc, char* argv[]) {

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

    unsigned int fd;
    for (fd = 0; fd < flim.rlim_max; fd++) {
        close(fd);
    }

    chdir("/");
//    fprint("Init start");
    openlog("Daemon Init", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "Init start");
    closelog();

    /*
     table - двумерный массив, pid - [1,0], где 1 - wait, 0 - re
     */

//    int** table = (int**) malloc(sizeof(int*) * count);

//    for (int i = 0; i < count; i++) {
//        table[i] = (int*) malloc(sizeof(int) * (string_length + 1));
//    }

    //    char* current_dir = get_current_dir_name();             // need to free()
        //readlink("/proc/pid/exe");

    read_conf(CONFIG);

    return 0;
}

