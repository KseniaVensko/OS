#include <stdio.h>
#include <stdlib.h>
#include <string.h>        // strdup - создает в куче строку и возвр указ на нее
#include <assert.h>
#include <wait.h>           //waitpid
#include <unistd.h>         // fork

#include <errno.h>
#include <log.h>

#define MAX_KEYS_COUNT 5
#define MAXPROC 10

char** str_split(char* str, const char delim) {
    char** result = 0;
    size_t count = 0;
    char* tmp = str;
    char* last_comma = 0;
    char del[2];
    del[0] = delim;
    del[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (delim == *tmp) {
            if (tmp != str) {           // TODO: вроде правильно, но проверь
                count++;
                last_comma = tmp;
            }
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (str + strlen(str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**) malloc(sizeof(char*) * count);

    if (result) {
        size_t idx  = 0;
        char* token = strtok(str, del);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);      // need to free()
            token = strtok(0, del);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

pid_t exec_program(char** keys, char* program_name) {
    pid_t pid = fork();
    if (pid > 0) {          //parent process
        return pid;
    }
    else if (pid == 0) {            // execv - с массивом аргументов
        execv(program_name, keys);
        fprint("I mustn`t be here");
        fprint(program_name);
        fprint(strerror(errno));
        return 0;                   // сюда не должны прийти
    }
    else {
        fprint("Error");
        return 0;                  // ошибка
    }
}

void create_pid_file(pid_t pid, char* name) {   // TODO: add errorhandlers
    // /tmp/name_of_file.pid <- pid

    char* chr = strrchr(name, '/');
    char filename_dest[50];
    strcpy(filename_dest, "/tmp/pids");        //TODO: don`t forget to change
    strcat(filename_dest, chr);
    strcat(filename_dest, ".pid");
    FILE* pid_file = fopen (filename_dest, "a+");
    fprintf(pid_file, "%d\n", pid);
    fflush(pid_file);
    fclose(pid_file);

    return;
}

void follow_childs(pid_t* pid_list, char* r_w_list, int count, char **keys) {
    pid_t cpid;
    fprint("I follow you, baby");
    while(count) {
        cpid = waitpid(-1, NULL, 0);
        int i;
        for(i = 0; i < MAXPROC; ++i) {
            if (pid_list[i] == cpid) {
                if (r_w_list[i] == 'r') {
                    pid_t pid = exec_program(keys, keys[0]);
                    if (pid != 0) {
                        pid_list[i] = pid;
                        r_w_list[i] = 'w';
                        create_pid_file(pid, keys[0]);
                    }
                }
                else {
                    pid_list[i] = 0;
                    count --;
                }
                break;
            }
        }
    }
}

int read_conf(char *path, pid_t* pid_list, char* r_w_list, char** global_keys) {
    //char* buf = (char*) malloc(sizeof(char) * 4000);
    char buf[500];
    char** parse_str;
    char** keys;
    int keys_count = 0;
    int child_count = 0;

    FILE *ptr = fopen(path, "r");

    while(1) {
        if(child_count == MAXPROC) {
            // to log and
            break;
        }
        if (!fgets(buf, sizeof(buf), ptr)) {
            if (feof(ptr) != 0) {                  // файл закончился
                fprint("End of config file");
                break;
            }
            else {                                // ошибка
                fprint("error");
                break;
            }
        }
        parse_str = str_split(buf, ' ');
        if (parse_str) {
            char* program_name = strdup(*(parse_str));
            keys = (char**) malloc(sizeof(char*) * MAX_KEYS_COUNT);
            keys[keys_count++] = program_name;

            int i;
            for (i = 1; *(parse_str + i); i++) {
                if (parse_str[i][0] == '-') {
                    if (keys_count < MAX_KEYS_COUNT) {
                        keys[keys_count++] = strdup(*(parse_str + i));
                    }
                }
                else if (*(*(parse_str + i)) == 'w' || *(*(parse_str + i)) == 'r') {
                    keys[keys_count] = NULL;
                    pid_t pid = exec_program(keys, program_name);

                    pid_list[child_count] = pid;
                    r_w_list[child_count] = *(*(parse_str + i));

                    if (*(*(parse_str + i)) == 'r') {
                        int k;
                        for(k = 0; k <= keys_count; ++k) {
                            if (keys[k] != NULL) {
                                global_keys[k] = strdup(keys[k]);
                            }
                            else {
                                global_keys[k] = NULL;
                            }
                        }

                    }
                    child_count++;

                    if (pid != 0) {
                        create_pid_file(pid, program_name);
                    }
                }
                else {
                    fprint("Wrong symbol");
                    // сломаться или проигнорировать
                }
                free(*(parse_str + i));
            }

            free(program_name);
//            int p;
//            for (p = 0; p < keys_count - 1; ++p) {
//                free(keys[p]);
//            }
            keys_count = 0;
        }
        else {
            fprint("hnya");
        }
    }
    free(parse_str);
    free(keys);
    fclose(ptr);
    return child_count;
}               // считали конфиг
