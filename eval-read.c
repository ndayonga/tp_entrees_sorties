#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "stdes.h"

#define NB_LITTLE_ELT (1ULL << 22)
#define NB_BIG_ELT (1 << 12)
#define BIG_ELT_SIZE (3*BUFFER_SIZE)
#define NB_RUN 5

char temp[BUFFER_SIZE];
char temp2[BIG_ELT_SIZE];

double little_read_stdes() {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    char c;
    IOBUF_FILE *f = iobuf_open("toread1", 'R');
    for (unsigned long i = 0; i < NB_LITTLE_ELT; i++) {
        int r = iobuf_read(&c, 1, 1, f);
        if (r <= 0) return 56;
    }
    iobuf_close(f);

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}

double little_read_syscall() {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    char c;
    int f = open("toread1", O_RDONLY);
    for (unsigned long i = 0; i < NB_LITTLE_ELT; i++) {
        int r = read(f, &c, 1);
        if (r <= 0) return 56;
    }
    close(f);

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}

double big_read_stdes() {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    IOBUF_FILE *f = iobuf_open("toread2", 'R');
    for (unsigned long i = 0; i < NB_BIG_ELT; i++) {
        int r = iobuf_read(temp2, 1, BIG_ELT_SIZE, f);
        if (r < BIG_ELT_SIZE) return 56;
    }
    iobuf_close(f);

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}

double big_read_syscall() {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    int f = open("toread2", O_RDONLY);
    for (unsigned long i = 0; i < NB_BIG_ELT; i++) {
        int r = read(f, temp2, BIG_ELT_SIZE);
        if (r < BIG_ELT_SIZE) return 56;
    }
    close(f);

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}

int main(int argc, char const *argv[])
{
    /* Experience 1 */

    int f = open("toread1", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    for (unsigned int i = 0; i < BUFFER_SIZE; i++)
        temp[i] = 'r';
    for (unsigned int i = 0; i < NB_LITTLE_ELT / BUFFER_SIZE; i++)
        write(f, temp, BUFFER_SIZE);
    close(f);


    double sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += little_read_stdes();
    }
    sum_time /= NB_RUN;
    
    iobuf_printf("======== TEST LITTLE READ =========\n");
    iobuf_printf("STDES   : Average wall time : %f\n", sum_time);

    sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += little_read_syscall();
    }
    sum_time /= NB_RUN;

    iobuf_printf("SYSCALL : Average wall time : %f\n", sum_time);


    iobuf_flush(stdout);


    /* Experience 2 */

    f = open("toread2", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    for (unsigned int i = 0; i < BIG_ELT_SIZE; i++)
        temp2[i] = 's';
    for (unsigned int i = 0; i < NB_BIG_ELT; i++)
        write(f, temp2, BIG_ELT_SIZE);
    close(f);

    sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += big_read_stdes();
    }
    sum_time /= NB_RUN;
    
    iobuf_printf("======== TEST BIG READ =========\n");
    iobuf_printf("STDES   : Average wall time : %f\n", sum_time);

    sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += big_read_syscall();
    }
    sum_time /= NB_RUN;

    iobuf_printf("SYSCALL : Average wall time : %f\n", sum_time);

    iobuf_flush(stdout);
    return 0;
}
