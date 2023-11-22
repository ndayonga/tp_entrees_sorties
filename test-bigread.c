#include "stdes.h"

char buf[9*BUFFER_SIZE];

int main(int argc, char const *argv[])
{
    int i;

    // on écrit trois éléments de 3*BUFFER_SIZE
    for (i = 0; i < 3*BUFFER_SIZE; i++) buf[i] = 'a';
    for (; i < 6*BUFFER_SIZE; i++) buf[i] = 'b';
    for (; i < 8*BUFFER_SIZE; i++) buf[i] = 'c'; // oups :
    // on s'arrête avant la fin du troisième 

    // On écrit 2 éléments et un bout du troisième.
    IOBUF_FILE *f = iobuf_open("totest.txt", 'W');
    int r = iobuf_write(buf, 1, 8*BUFFER_SIZE, f);
    iobuf_printf("nb_ecriture = %d\n", r);

    iobuf_close(f);
    f = iobuf_open("totest.txt", 'R');

    // on essaie de lire trois éléments
    int r1 = iobuf_read(buf, 3*BUFFER_SIZE, 3, f);
    iobuf_printf("nb elemt de %d octets lus : %d %c %c\n", 3*BUFFER_SIZE, r1, buf[0], buf[r1*3*BUFFER_SIZE-1]);
    // on devrait en avoir 2

    // on devrait pouvoir lire les 2*BUFFER_SIZE octets pas encore lus dans le fichier
    int r2 = iobuf_read(buf, BUFFER_SIZE, 3, f);
    iobuf_printf("nb elemt de %d lus ? %d %c\n", BUFFER_SIZE, r2, buf[0]);

    // nombre total d'octets lus
    iobuf_printf("nb octets total : %d\n", 3*BUFFER_SIZE*r1+BUFFER_SIZE*r2);

    iobuf_flush(stdout);
    iobuf_close(f);
}
