#ifndef READ_FILES_H
#define READ_FILES_H

char** str_split(char* str, const char delim);
pid_t exec_program(char** keys, char* program_name, char mode);
void create_pid_file(pid_t pid, char* name);
void read_conf(char *path);

#endif // READ_FILES_H
