#include "stdes.h"

char buf[9*BUFFER_SIZE];

int main(int argc, char const *argv[])
{
    int i;
    for (i = 0; i < 3*BUFFER_SIZE; i++) buf[i] = 'a';
    for (; i < 6*BUFFER_SIZE; i++) buf[i] = 'b';
    for (; i < 9*BUFFER_SIZE; i++) buf[i] = 'c';

    IOBUF_FILE *f = iobuf_open("totest2.txt", 'W');
    int r = iobuf_write(buf, 9*BUFFER_SIZE, 1, f);
    iobuf_printf("nb_ecriture +gd que le buffer = %d\n", r);

    r = iobuf_write(buf, BUFFER_SIZE-52, 9, f);
    iobuf_printf("nb_ecriture à peine plus petit que buffer_size = %d\n", r);

    iobuf_flush(stdout);
    iobuf_close(f);
}