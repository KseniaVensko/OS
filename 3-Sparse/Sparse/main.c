#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // write()

// argv[1] - name of file to store

int main(int argc, char *argv[]) {
    char c;
    int count=0;
    int ou;
    int pos;


//    int in = open("one", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
//    printf("%s %d\n", "fd is", in);
//    int ou = open("two", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
//    printf("%s %d\n", "fd is", ou);
//    int e = open("three", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
//    printf("%s %d\n", "fd is", e);
    ou = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
//printf("%s %d\n", "fd is", ou);
//    if ( ou == -1 ) {
//        perror("Cannot open output file\n");
//        exit(1);
//    }
//printf("%s %d\n", "fd is", ou);
//    int w;
//    while ((w = read(STDIN_FILENO, &c, 1)) == 0);
//    printf("%d", w);
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 0) {
            ++count;
            printf("%d ", count);
        }
//        else {
//            if (count == 0) {
                write(ou, &c, 1);
//                printf("%02X", c);
//            }
//            else {
//                pos = lseek(ou, count, SEEK_CUR);
//                write(ou, &c, 1);
//                count = 0;
//            }
 //       }
        //putchar(c);
        //printf("%s ", argv[1]);
        //write(STDOUT_FILENO, &c, 1);
    }
//    char* buf="World";
//    int p = write(ou, buf, 6);
//    if (p != 6) {
//            write(2, "There was an error writing to testfile.txt\n", 43);
//            printf("%d\n", p);
//            perror("Cann`t write in output file\n");
//            return -1;
//        }

    int er = close(ou);
    if (er == -1) {
        perror("Cann`t close output file\n");
        exit(1);
    }
    return 0;
}

