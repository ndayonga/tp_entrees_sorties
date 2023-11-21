#include "stdes.h"

int main(int argc, char const *argv[])
{
    char buf[9*BUFFER_SIZE];
    int i;
    for (i = 0; i < 3*BUFFER_SIZE; i++) buf[i] = 'a';
    for (; i < 6*BUFFER_SIZE; i++) buf[i] = 'b';
    for (; i < 8*BUFFER_SIZE; i++) buf[i] = 'c'; // oups !

    IOBUF_FILE *f = iobuf_open("totest2.txt", 'W');
    int r = iobuf_write(buf, 8*BUFFER_SIZE, 1, f);
    iobuf_printf("nb_ecriture = %d\n", r);

    iobuf_flush(stdout);
    iobuf_close(f);
}