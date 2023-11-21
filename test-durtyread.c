#include "stdes.h"

int main(int argc, char const *argv[])
{
    char buf[9216];
    int i;
    for (i = 0; i < 3*1024; i++) buf[i] = 'a';
    for (; i < 6*1024; i++) buf[i] = 'b';
    for (; i < 9000; i++) buf[i] = 'c'; // oups !

    IOBUF_FILE *f = iobuf_open("totest.txt", 'W');
    int r = iobuf_write(buf, 1, 9000, f);
    iobuf_printf("nb_ecriture = %d\n", r);

    iobuf_close(f);
    f = iobuf_open("totest.txt", 'R');

    r = iobuf_read(buf, 3072, 3, f);
    iobuf_printf("nb elemt de 1024 octets lus : %d %c %c\n", r, buf[0], buf[r*3072-1]);

    r = iobuf_read(buf, 2856, 1, f);
    iobuf_printf("elt lu ? %d %c\n", r, buf[0]);


    iobuf_flush(stdout);
    iobuf_close(f);
}
