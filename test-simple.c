#include "stdes.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc != 2)
        return 1;

    IOBUF_FILE *f = iobuf_open(argv[1], 'W');
    iobuf_fprintf(f, "du %c texte ! %s %d %d %d %d", 'a', "et encore!!!", 1000, 1234567, 0, -2);
    iobuf_close(f);
    
    return 0;
}
