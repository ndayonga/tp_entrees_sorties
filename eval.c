#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "stdes.h"

#define CLOCK_TYPE CLOCK_PROCESS_CPUTIME_ID    // ou alors CLOCK_REALTIME
#define NB_RUN 5

#define NB_LITTLE_ELT (1U << 22)
#define NB_BIG_ELT (1U << 15)

struct timespec begin, end;

void fct_prologue() {
    clock_gettime(CLOCK_TYPE, &begin);
}

double fct_epilogue() {
    clock_gettime(CLOCK_TYPE, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}
