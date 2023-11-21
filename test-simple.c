#include "stdes.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc != 2)
        return 1;

    IOBUF_FILE *f = iobuf_open(argv[1], 'W');
    iobuf_fprintf(f, "du %c texte ! %s %d %d %d %d %d", 'a', "et_encore!!!", 1, 23485, 1, 1, -1);
    iobuf_close(f);

    iobuf_printf("Done %d !\n", 5);
    iobuf_printf("Done %d !\n", -5);
    iobuf_printf("Done %d !\n", 74);

    iobuf_open(argv[1], 'R');

    char a;
    char s[14];
    int i1;
    int i2;
    int i3;
    int i4;
    int i5;
    iobuf_fscanf(f, "du %c texte ! %s %d %d %d %d %d", &a, s, &i1, &i2, &i3, &i4, &i5);
    iobuf_printf("char lu : %c\n", a);
    iobuf_printf("string lu : %s\n", s);
    iobuf_printf("int 1 à 5 lus : %d %d %d %d %d\n", i1, i2, i3, i4, i5);

    iobuf_flush(stdout);
    
    return 0;
}
