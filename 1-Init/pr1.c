#include <unistd.h>     // sleep()
#include <stdlib.h>     //strtol
#include <log.h>

int main(int argc,  char** argv) {
    if (argc > 2 || argc < 2) {
        return 1;
    }
    char *end;
    long int i = strtol(argv[1], &end, 10);
    int j;
    for (j = 0; j < i; ++j) {
        fprint("I`m working");
        sleep(10);
    }
    return 0;
}
