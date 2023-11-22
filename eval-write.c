#include "eval.c"

#define BIG_ELT_SIZE 1012

char temp[NB_BIG_ELT * BIG_ELT_SIZE];

double little_write_stdes() {
    fct_prologue();


    IOBUF_FILE *f = iobuf_open("towrite1", 'W');
    char c = '1';
    for (size_t i = 0; i < NB_LITTLE_ELT; i++)
        iobuf_write(&c, 1, 1, f);
    iobuf_close(f);


    return fct_epilogue();
}

double little_write_syscall() {
    fct_prologue();

    int f = open("towrite2", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    char c = '2';
    for (size_t i = 0; i < NB_LITTLE_ELT; i++)
        write(f, &c, 1);
    close(f);

    return fct_epilogue();
}


double big_write_stdes() {
    fct_prologue();

    IOBUF_FILE *f = iobuf_open("towrite3", 'W');
    for (unsigned int i = 0; i < NB_BIG_ELT * BIG_ELT_SIZE; i++)
        temp[i] = '3';
    iobuf_write(temp, BIG_ELT_SIZE, NB_BIG_ELT, f);
    iobuf_close(f);

    return fct_epilogue();
}

double big_write_syscall() {
    fct_prologue();

    int f = open("towrite4", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    for (unsigned int i = 0; i < NB_BIG_ELT * BIG_ELT_SIZE; i++)
        temp[i] = '4';
    write(f, temp, BIG_ELT_SIZE * NB_BIG_ELT);
    close(f);

    return fct_epilogue();
}


int main(int argc, char *argv[]) {

    /* Expérience 1 */
    double sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += little_write_stdes();
    }
    sum_time /= NB_RUN;
    
    iobuf_printf("======== TEST LITTLE WRITE =========\n");
    iobuf_printf("STDES   : Average time : %f\n", sum_time);

    sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += little_write_syscall();
    }
    sum_time /= NB_RUN;

    iobuf_printf("SYSCALL : Average time : %f\n", sum_time);
    iobuf_flush(stdout);


    /* Expérience 2 */

    sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += big_write_stdes();
    }
    sum_time /= NB_RUN;
    
    iobuf_printf("======== TEST BIG WRITE =========\n");
    iobuf_printf("STDES   : Average time : %f\n", sum_time);

    sum_time = 0;
    for (int i = 0; i < NB_RUN; i++) {
        sum_time += big_write_syscall();
    }
    sum_time /= NB_RUN;

    iobuf_printf("SYSCALL : Average time : %f\n", sum_time);

    iobuf_flush(stdout);

    return 0;
}
