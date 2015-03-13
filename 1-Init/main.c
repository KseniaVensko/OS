#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/resource.h>       //getrlimit
#include <unistd.h>     // getppid, readlink
#include <stdlib.h>     // exit

#include <string.h>
#include <work_with_childs.h>
#include <log.h>
#define CONFIG "/home/ksenia/OS/build-1-Init/1-Init.conf"
#define MAXPROC 10

int main() {

    char conf[100];
    getcwd(conf, 100);
    strcat(conf, "/");
    strcat(conf, "1-Init.conf");
    //readlink("/proc/pid/exe");

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

    openlog("Daemon Init", LOG_PID | LOG_CONS, LOG_DAEMON);
    fprint("Init start");

    pid_t* pid_list = (pid_t*) malloc(sizeof(pid_t) * MAXPROC);
    char* r_w_list = (char*) malloc(sizeof(char) * MAXPROC);
    char** keys = (char**) malloc(sizeof(char**) * MAXPROC);

    int child_count = read_conf(conf, pid_list, r_w_list, keys);

    if (child_count != 0) {
        follow_childs(pid_list, r_w_list, child_count, keys);
    }
    //    int i;
    //    for (i = 0; i < MAXPROC; ++i) {
    //        free(keys[i]);
    //    }
    free(keys);
    free(pid_list);
    free(r_w_list);
    fprint("Init ends");
    closelog();
    return 0;
}

