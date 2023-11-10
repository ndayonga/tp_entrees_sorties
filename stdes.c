#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "stdes.h"

/* mode: 'R' = lecture, 'W' = écriture */
IOBUF_FILE *iobuf_open(const char *nom, char mode) {
    if (mode != 'R' && mode != 'W') return NULL;

    // open file
    int flags = (mode == 'R') ? O_RDONLY : O_CREAT | O_WRONLY;
    int res = open(nom, flags);
    if (res < 0) return NULL;

    // allocation structure
    IOBUF_FILE *f = mmap(NULL, sizeof(IOBUF_FILE), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
    if (f == MAP_FAILED) return NULL;

    // initialisation
    f->file_desc = res;
    f->mode = mode;
    f->buf_first = 0;
    f->buf_end = 0;

    return f;
}

int iobuf_close(IOBUF_FILE *f) {
    return 0;
}


int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f) {
    return 0;
}


int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f) {
    return 0;
}

int vider(IOBUF_FILE *f) {
    return 0;
}

int iobuf_fprintf(IOBUF_FILE *f, const char *format, ...);
/* directly in stdout */
int iobuf_printf(const char *format, ...);
int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...);
