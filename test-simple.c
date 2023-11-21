#include "stdes.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc != 2)
        return 1;

    IOBUF_FILE *f = iobuf_open(argv[1], 'W');
    if (f == NULL) return 3;

    iobuf_fprintf(f, "du %c texte ! %s %d %d %d %d %d", 'a', "et_encore!!!", 1, 23485, 1, 1, -1);
    iobuf_close(f);

    iobuf_printf("Done %d !\n", 5);
    iobuf_printf("Done %d !\n", -5);
    iobuf_printf("Done %f !\n", -1414213.5623730951);

    f = iobuf_open(argv[1], 'R');
    if (f == NULL) return 4;

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
    iobuf_close(f);
    
    return 0;
}
