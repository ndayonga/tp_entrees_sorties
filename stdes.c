#include <stdlib.h>
#include "stdes.h"


/* mode: 'R' = lecture, 'W' = écriture */
IOBUF_FILE *iobuf_open(const char *nom, char mode) {
    return NULL;
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
