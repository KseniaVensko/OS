#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // write()

// argv[1] - name of file to store

int main(int argc, char *argv[]) {
    char c;
    int count=0;
    int ou;

    ou = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
    if ( ou == -1 ) {
        perror("Cannot open output file\n");
        exit(1);
    }
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 0) {
            ++count;
        }
        else {
            if (count == 0) {
                write(ou, &c, 1);
            }
            else {
                lseek(ou, count, SEEK_CUR);
                write(ou, &c, 1);
                count = 0;
            }
        }
    }

    int er = close(ou);
    if (er == -1) {
        perror("Cann`t close output file\n");
        exit(1);
    }
    return 0;
}

