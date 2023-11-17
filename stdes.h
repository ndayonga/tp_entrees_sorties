#ifndef _STDES_H
#define _STDES_H

#define BUFFER_SIZE 1024

/**
 * un fichier est composé de :
 *  - un descripteur de fichier
 *  - un mode : lecture ou ecriture
 *  - un buffer
 *  - la taille du buffer
 *  - la position où lire / écrire
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

/* mode: 'L' = lecture, 'E' = écriture */
IOBUF_FILE *iobuf_open(const char *nom, char mode);
int iobuf_close(IOBUF_FILE *f);
int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_flush(IOBUF_FILE *f);

int iobuf_fprintf(IOBUF_FILE *f, const char *format, ...);
/* directly in stdout */
int iobuf_printf(const char *format, ...);
int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...);

#endif
