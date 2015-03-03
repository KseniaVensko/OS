#include <stdio.h>
#include <stdlib.h>
#include <string.h>        // strdup - создает в куче строку и возвр указ на нее
#include <assert.h>
#include <wait.h>           //waitpid
#include <unistd.h>         // fork

#include <errno.h>
#include <log.h>

#define MAX_KEYS_COUNT 5

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
            count++;
            last_comma = tmp;
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

pid_t exec_program(char** keys, char* program_name, char mode) {
    pid_t pid = fork();
    if (pid > 0) {          //parent process
        //        if (mode == 'w') {
        //            waitpid(pid,);
        //            return pid;
        //        }
        return pid;
    }
    else if (pid == 0) {            // execv - с массивом аргументов
        execv(program_name, keys);
        fprint("I mustn`t be here");
        //fprint (errno);
        fprint(program_name);
        fprint(strerror(errno));
        exit(1);
        return 0;                   // сюда не должны прийти
    }
    else {
        fprint("Error");
        return 0;                  // ошибка
    }
}

void create_pid_file(pid_t pid, char* name) {   // TODO: add errorhandlers
    // /tmp/name_of_file.pid <- pid
    char filename_dest[50];

    strcpy(filename_dest, "/tmp/pids/");        //TODO: don`t forget to change
    strcat(filename_dest, name);
    strcat(filename_dest, ".pid");
    FILE* pid_file = fopen (filename_dest, "a+");
    fprintf(pid_file, "%d\n", pid);
    fflush(pid_file);
    fclose(pid_file);

    return;
}

void read_conf(char *path) {
    //char* buf = (char*) malloc(sizeof(char) * 4000);
    char buf[500];
    char** parse_str;
    char** keys;
//    char* program_name;
    int keys_count = 0;

    FILE *ptr = fopen(path, "r");

    while(1) {
        if (!fgets(buf, sizeof(buf), ptr)) {
            if (feof(ptr) != 0) {                  // файл закончился
fprint("");
                break;
            }
            else {                                // ошибка
                fprint("error");
                break;
            }
        }
fprint("buffer");
fprint(buf);
        parse_str = str_split(buf, ' ');
        if (parse_str) {
            char* program_name = strdup(*(parse_str));

            keys = (char**) malloc(sizeof(char*) * MAX_KEYS_COUNT);
            keys[keys_count++] = program_name;
fprint("name");
fprint(program_name);
            int i;
            for (i = 1; *(parse_str + i); i++) {
                if (parse_str[i][0] == '-') {
                    if (keys_count < MAX_KEYS_COUNT) {
                        keys[keys_count++] = strdup(*(parse_str + i));
                    }
                }
                else if (*(*(parse_str + i)) == 'w' || *(*(parse_str + i)) == 'r') {
                    keys[keys_count] = NULL;
fprint("keys");
for (i = 0; i < keys_count; ++i) {
     fprint(keys[i]);
}
                    pid_t pid = exec_program(keys, program_name, parse_str[i][0]);
                    //write_to_table();
                    if (pid != 0) {
fprint("parent should create pid file here");
                        //create_pid_file(pid, program_name);
                    }
                }
                else {
                    // сломаться или проигнорировать
                }
                free(*(parse_str + i));
            }

            free(program_name);
            for (i = 0; i < keys_count; ++i) {
                free(keys[keys_count]);
            }
            keys_count = 0;
        }
    else {
            fprint("hnya");
        }
    }
    free(parse_str);
    free(keys);
    fclose(ptr);
}
