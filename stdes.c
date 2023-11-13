#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "stdes.h"

/* Ecriture dans le buffer système d'une partie ou de tout ce qui se trouve dans le tampon utilisateur */
void ecrire_buffer_systeme(IOBUF_FILE *f, void *p, int size){
    // f->buf_pos = f->buf_pos + size;
    memcpy((char*)f->buf + f->buf_pos, (char*)p, size );
    f->buf_pos = f->buf_pos + size;
}

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
    // f->buf_pos = 0;
    return f;
}

int iobuf_close(IOBUF_FILE *f) {
    if (f == NULL) return -1;
    if (f->buf_pos > 0) vider(f);
    if (munmap(f, sizeof(IOBUF_FILE)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    return 0;
}


int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f) {
    
}


int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f) {
    if (f->mode != 'W' || p == NULL){
        return -1;
    }
    int size = taille * nbelem;

    if ((f->buf_pos + size) <= BUFFER_SIZE){
        ecrire_buf_systeme(f, p, size);
        return size;
    }

    else{
        int size_written = 0;    // Taille de ce qui a été écrit dans le buffer système
        int n_elem_restant = nbelem;
        int n_elem_buf = (BUFFER_SIZE - f->buf_pos)/taille;    
        while(taille*n_elem_restant > (BUFFER_SIZE - f->buf_pos)){
            n_elem_buf = (BUFFER_SIZE - f->buf_pos)/taille;      // Nombre d'éléments qui pourront être écrit dans le buffer
            ecrire_buffer_systeme(&f, p+size_written, taille*n_elem_buf);
            if (vider(f) == -1) return -1;
            size_written += taille*n_elem_buf;
            n_elem_restant -= n_elem_buf;
        }
        if(n_elem_restant != 0){
            ecrire_buffer_systeme(&f, p+size_written, taille*n_elem_restant);
            size_written += taille*n_elem_restant;
        }

        return size_written;
    }
}

int vider(IOBUF_FILE *f) {
    if (f == NULL) return -1;
    if (f->buf_pos > 0){
        int size = f->buf_pos;
        if(write(f->file_desc, f->buf, size) == -1) return -1;
        f->buf_pos = 0;
    }
    return 0;
}

int iobuf_fprintf(IOBUF_FILE *f, const char *format, ...);
/* directly in stdout */
int iobuf_printf(const char *format, ...);
int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...);
