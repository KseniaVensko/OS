#ifndef WORK_WITH_CHILDS_H
#define WORK_WITH_CHILDS_H

char** str_split(char* str, const char delim);
pid_t exec_program(char** keys, char* program_name);
void create_pid_file(pid_t pid, char* name);
void follow_childs(pid_t* pid_list, char* r_w_list, int, char**);
int read_conf(char *path, pid_t*, char*, char**);

#endif // WORK_WITH_CHILDS_H
