#ifndef _STDES_H
#define _STDES_H

#include <stdarg.h>

#define BUFFER_SIZE 1024

/**
 * un fichier est composé de :
 *  - un descripteur de fichier
 *  - un mode : lecture ou ecriture ('R' ou 'W' uniquement)
 *  - un buffer (de BUFFER_SIZE octets)
 *  - la taille effective du buffer (<= BUFFER_SIZE)
 *  - la position où lire (uniquement)
 */
struct _IOBUF_FILE {
    int file_desc;
    char mode; // 'R' ou 'W'
    char buf[BUFFER_SIZE];
    unsigned int buf_size;
    unsigned int buf_pos; // uniquement pour la lecture (ici c'est un offset)
};

typedef struct _IOBUF_FILE IOBUF_FILE;

extern IOBUF_FILE *stdout;
extern IOBUF_FILE *stderr;

/* mode: 'R' = lecture, 'W' = écriture */
IOBUF_FILE *iobuf_open(const char *nom, char mode);
int iobuf_close(IOBUF_FILE *f);
int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_flush(IOBUF_FILE *f);

int iobuf_vprintf(IOBUF_FILE *f, const char *format, va_list args);
int iobuf_fprintf(IOBUF_FILE *f, const char *format, ...);
/* directly in stdout */
int iobuf_printf(const char *format, ...);
int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...);

#endif
