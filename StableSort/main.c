#include <stdio.h>
#include <stdlib.h> // malloc
#include <errno.h>
#include <string.h>
#include <ctype.h> // isspace

#define DEFAULTCOUNT 1000
typedef enum { false, true } bool;

int compare_doubles (const void *a, const void *b) {
    const double *da = (const double *) a;
    const double *db = (const double *) b;

    return (*da > *db) - (*da < *db);
}

bool increase(double* arr, int count) {
    double* tmp = (double*) realloc(arr, count*sizeof(double));
    if (tmp == NULL) {
        printf("Can not allocate memory");
        return false;
    }
    arr = tmp;
    return true;
}

int handleFile(char* file, double* arr, int common_count) {
    FILE* in = fopen(file, "r");

    if ( in == NULL ) {
        printf("%s %s : %s\n", "Cannot open input file", file, strerror(errno));
        return 0;
    }

    int n;
    double d;

    while((n = fscanf(in, "%lf", &d)) != -1) {
        printf("n : %d\n", n);
        if (n == 0) {                   // 0 возвращает, если там некорректные символы
            int c;
            c = fgetc(in);
            while (!isspace(c) && c != EOF) {
                c = fgetc(in);
            }
            continue;
        }
        if (common_count >= DEFAULTCOUNT) {
            printf("Sorry, not enough memory");
            return 0;
        }
        arr[common_count++] = d;
    }
    return common_count;
}

int main(int argc, char *argv[])
{
    int i;

    double common_array[1000];
    int common_count=0;

    for (i=1; i<argc-1; ++i) {
        common_count=handleFile(argv[i], common_array, common_count);
    }

    qsort(common_array, common_count, sizeof(double), compare_doubles);
    FILE *out = fopen(argv[argc-1], "w");

    int k;
    if ( out == NULL ) {
        printf("%s %s : %s\n", "Cannot open output file", argv[argc-1], strerror(errno));

        for (k=0; k<common_count; ++k) {
            printf("%f ", common_array[k]);
        }

        return 0;
    }

    for (k=0; k<common_count; ++k) {
        int stat = fprintf(out, "%f ", common_array[k]);

        if (stat < 0) {
            printf("Can't write to output file: %s\n", strerror(errno));
            fclose(out);
            return 0;
        }
    }

    int stat = fclose(out);

    if (stat == -1) {
        printf("So sad, I can not close output file: %s", strerror(errno));
    }

    return 0;
}
