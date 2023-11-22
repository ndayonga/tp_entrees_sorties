#include "eval.c"

#define BIG_ELT_SIZE 1012

char temp[NB_BIG_ELT * BIG_ELT_SIZE];

void little_write_stdes() {
    IOBUF_FILE *f = iobuf_open("towrite1", 'W');
    char c = '1';
    for (size_t i = 0; i < NB_LITTLE_ELT; i++)
        iobuf_write(&c, 1, 1, f);
    iobuf_close(f);
}

void little_write_syscall() {
    int f = open("towrite2", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    char c = '2';
    for (size_t i = 0; i < NB_LITTLE_ELT; i++)
        write(f, &c, 1);
    close(f);
}


void big_write_stdes() {
    IOBUF_FILE *f = iobuf_open("towrite3", 'W');
    for (unsigned int i = 0; i < NB_BIG_ELT * BIG_ELT_SIZE; i++)
        temp[i] = '3';
    iobuf_write(temp, BIG_ELT_SIZE, NB_BIG_ELT, f);
    iobuf_close(f);
}

void big_write_syscall() {
    int f = open("towrite4", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    for (unsigned int i = 0; i < NB_BIG_ELT * BIG_ELT_SIZE; i++)
        temp[i] = '4';
    write(f, temp, BIG_ELT_SIZE * NB_BIG_ELT);
    close(f);
}


int main(int argc, char *argv[]) {

    /* Expérience 1 */
    double av_time = eval_test(little_write_stdes);
    iobuf_printf("======== TEST LITTLE WRITE =========\n");
    iobuf_printf("STDES   : Average time : %f\n", av_time);

    av_time = eval_test(little_write_syscall);
    iobuf_printf("SYSCALL : Average time : %f\n", av_time);
    iobuf_flush(stdout);


    /* Expérience 2 */

    av_time = eval_test(big_write_stdes);    
    iobuf_printf("======== TEST BIG WRITE =========\n");
    iobuf_printf("STDES   : Average time : %f\n", av_time);

    av_time = eval_test(big_write_syscall);
    iobuf_printf("SYSCALL : Average time : %f\n", av_time);

    iobuf_flush(stdout);

    return 0;
}
