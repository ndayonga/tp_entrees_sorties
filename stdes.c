#include <stdarg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
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
    f->buf_size = 0;
    f->buf_pos = 0;
    return f;
}

int iobuf_close(IOBUF_FILE *f) {
    if (f == NULL) return -1;
    if (iobuf_flush(f) < 0) return -1;
    if (munmap(f, sizeof(IOBUF_FILE)) == -1) return -1;
    return 0;
}


int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f) {
    // nb d'octets lus et à lire
    unsigned int nboctetslus = 0; // aussi position pour p
    unsigned int nboctetsAlire = taille*nbelem;


    while (nboctetslus < nboctetsAlire) { // tant qu'on a encore des octets à lire
        if (f->buf_pos == f->buf_size) { // si le buffer est entierement lu : appel systeme
            int res = read(f->file_desc, f->buf, BUFFER_SIZE);
            f->buf_pos = 0;
            f->buf_size = 0;
            if (res <= 0) break;
            f->buf_size = res;
        }
        
        // on lit dans le buffer tant qu'on peut et qu'on veut.
        while (f->buf_pos < f->buf_size && nboctetslus < nboctetsAlire) {
            ((char*)p)[nboctetslus] = ((char*)f->buf)[f->buf_pos]; // recopie
            f->buf_pos++;
            nboctetslus++;
        }
    }

    // si des octets en trop
    unsigned int nboctets_eltincomplet = nboctetslus % taille;

    // on reremplit f->buf avec les octets en trop
    unsigned int i = 0;
    while (i < nboctets_eltincomplet && i < BUFFER_SIZE) {
        ((char*)f->buf)[i] = ((char*)p)[nboctetslus-nboctets_eltincomplet+i];
        i++;
    }
    f->buf_pos = 0;
    f->buf_size = i;
    
    // on remet le curseur du fichier au bon endroit (si besoin est)
    if (i < nboctets_eltincomplet) {
        lseek(f->file_desc, -(nboctets_eltincomplet-i), SEEK_CUR);
    }
    
    return nboctetslus / taille;
}


int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f) {
    return 0;
}

int iobuf_flush(IOBUF_FILE *f) {
    if (f == NULL) return -1;
    if (f->buf_size > 0) {
        if(write(f->file_desc, f->buf, f->buf_size) < 0) return -1;
        f->buf_size = 0;
    }
    return 0;
}

int iobuf_fprintf(IOBUF_FILE *f, const char *format, ...) {
    // fwrite octet par octet

    return 0;
}

/* directly in stdout */
int iobuf_printf(const char *format, ...) {
    return iobuf_fprintf(stdout, format);
}

int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...) {
    return 0;
}
