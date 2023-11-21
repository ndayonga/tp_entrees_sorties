#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "stdes.h"

#define NB_LITTLE_ELT (1ULL << 21)
#define NB_BIG_ELT (1 << 13)
#define BIG_ELT_SIZE 1012
#define NB_RUN 5

char temp[NB_BIG_ELT * BIG_ELT_SIZE];

double little_write_stdes() {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);


    IOBUF_FILE *f = iobuf_open("towrite1", 'W');
    char c = '1';
    for (size_t i = 0; i < NB_LITTLE_ELT; i++)
        iobuf_write(&c, 1, 1, f);
    iobuf_close(f);


    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}

double little_write_syscall() {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    int f = open("towrite2", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    char c = '2';
    for (size_t i = 0; i < NB_LITTLE_ELT; i++)
        write(f, &c, 1);
    close(f);

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}


double big_write_stdes() {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    IOBUF_FILE *f = iobuf_open("towrite3", 'W');
    for (unsigned int i = 0; i < NB_BIG_ELT * BIG_ELT_SIZE; i++)
        temp[i] = '3';
    iobuf_write(temp, BIG_ELT_SIZE, NB_BIG_ELT, f);
    iobuf_close(f);

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}

double big_write_syscall() {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    int f = open("towrite4", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    char temp[NB_BIG_ELT * BIG_ELT_SIZE];
    for (unsigned int i = 0; i < NB_BIG_ELT * BIG_ELT_SIZE; i++)
        temp[i] = '4';
    write(f, temp, BIG_ELT_SIZE * NB_BIG_ELT);
    close(f);

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}


int main(int argc, char *argv[]) {
    double sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += little_write_stdes();
    }
    sum_time /= NB_RUN;
    
    iobuf_printf("======== TEST LITTLE WRITE =========\n");
    iobuf_printf("STDES   : Average wall time : %f\n", sum_time);

    sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += little_write_syscall();
    }
    sum_time /= NB_RUN;

    iobuf_printf("SYSCALL : Average wall time : %f\n", sum_time);


    sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += big_write_stdes();
    }
    sum_time /= NB_RUN;
    
    iobuf_printf("======== TEST BIG WRITE =========\n");
    iobuf_printf("STDES   : Average wall time : %f\n", sum_time);

    sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += big_write_syscall();
    }
    sum_time /= NB_RUN;

    iobuf_printf("SYSCALL : Average wall time : %f\n", sum_time);

    iobuf_flush(stdout);

    return 0;
}
