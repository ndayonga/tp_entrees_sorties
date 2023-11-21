#include "stdes.h"

int main(int argc, char const *argv[])
{
    char buf[9*BUFFER_SIZE];
    int i;
    for (i = 0; i < 3*BUFFER_SIZE; i++) buf[i] = 'a';
    for (; i < 6*BUFFER_SIZE; i++) buf[i] = 'b';
    for (; i < 8*BUFFER_SIZE; i++) buf[i] = 'c'; // oups !

    IOBUF_FILE *f = iobuf_open("totest.txt", 'W');
    int r = iobuf_write(buf, 1, 8*BUFFER_SIZE, f);
    iobuf_printf("nb_ecriture = %d\n", r);

    iobuf_close(f);
    f = iobuf_open("totest.txt", 'R');

    int r1 = iobuf_read(buf, 3*BUFFER_SIZE, 3, f);
    iobuf_printf("nb elemt de %d octets lus : %d %c %c\n", 3*BUFFER_SIZE, r1, buf[0], buf[r1*3*BUFFER_SIZE-1]);

    int r2 = iobuf_read(buf, BUFFER_SIZE, 3, f);
    iobuf_printf("nb elemt de %d lus ? %d %c\n", BUFFER_SIZE, r2, buf[0]);

    iobuf_printf("nb octets total : %d\n", 3*BUFFER_SIZE*r1+BUFFER_SIZE*r2);

    iobuf_flush(stdout);
    iobuf_close(f);
}