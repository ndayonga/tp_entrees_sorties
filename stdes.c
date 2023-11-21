#include <sys/mman.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include "stdes.h"


/* Entree et sorties standards */
IOBUF_FILE _stdin = {
    .file_desc = 0, .mode = 'R',
    .buf_pos = 0, .buf_size = 0
};

IOBUF_FILE _stdout = {
    .file_desc = 1, .mode = 'W',
    .buf_pos = 0, .buf_size = 0
};

IOBUF_FILE _stderr = {
    .file_desc = 2, .mode = 'W', 
    .buf_pos = 0, .buf_size = 0
};


IOBUF_FILE *stdin = &_stdin;
IOBUF_FILE *stdout = &_stdout;
IOBUF_FILE *stderr = &_stderr;




/* Fonctions */


/* Ouvrir un fichier */
/* mode: 'R' = lecture, 'W' = écriture */
IOBUF_FILE *iobuf_open(const char *nom, char mode) {
    if (mode != 'R' && mode != 'W') return NULL;

    // open file
    int flags = (mode == 'R') ? O_RDONLY : O_CREAT | O_WRONLY;
    int res = open(nom, flags, S_IRUSR | S_IWUSR);
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

/* Ferme un fichier */
int iobuf_close(IOBUF_FILE *f) {
    if (iobuf_flush(f) < 0) return -1;
    if (munmap(f, sizeof(IOBUF_FILE)) < 0) return -1;
    return 0;
}

/* Ecrire dans p nbelem * taille octets lu dans f */
int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f) {
    if (f->mode != 'R') return -1;

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
    if (nboctets_eltincomplet > 0) {
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
            int s = lseek(f->file_desc, 0, SEEK_CUR) - nboctets_eltincomplet + i;
            lseek(f->file_desc, s, SEEK_SET);
        }
    }
    
    return nboctetslus / taille;
}


/* Ecriture dans le buffer système */
void ecrire_buffer_systeme(IOBUF_FILE *f, const void *p, int size) {
    for (unsigned int i = f->buf_size; i < f->buf_size + size; i++) {
        ((char*)f->buf)[i] = ((char*)p)[i - f->buf_size];
    }
    f->buf_size = f->buf_size + size;
}

/* Ecrire dans f nbelem * taille octets lu dans p */
int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f) {
    if (f->mode != 'W') return -1;

    unsigned int taille_totale = taille * nbelem;

    // si tout rentre dans le buffer
    if (f->buf_size + taille_totale <= BUFFER_SIZE) {
        ecrire_buffer_systeme(f, p, taille_totale);
        return nbelem;
    }

    // sinon
    else {
        unsigned int size_written = 0;   // Nombre d'octets déjà écrits
        unsigned int n_elem_restant = nbelem;    // Nombre d'éléments à lire
        unsigned int n_elem_buf = 0;  // Nombre d'éléments à écrire dans le buffer

        // Tant que le nombre d'éléments non encore écrits ne rentre pas dans le buffer,
        // On remplit le buffer avec ce qu'on peut et on fait write avant de le vider
        while (taille*n_elem_restant > BUFFER_SIZE - f->buf_size) {
            n_elem_buf = (BUFFER_SIZE - f->buf_size) / taille;
            ecrire_buffer_systeme(f, p+size_written, taille*n_elem_buf);
            if (iobuf_flush(f) < 0) return nbelem - n_elem_restant;
            size_written += taille*n_elem_buf;
            n_elem_restant -= n_elem_buf;
        }

        // Si les éléments restants rentrent dans le buffer, on les met dans le buffer
        if (n_elem_restant != 0) {
            ecrire_buffer_systeme(f, p+size_written, taille*n_elem_restant);
            size_written += taille*n_elem_buf;
        }

        return nbelem;
    }
}


/* Forcer les ecritures sur disque */
int iobuf_flush(IOBUF_FILE *f) {
    if (f == NULL) return -1;
    if (f->buf_size > 0) {
        int res = write(f->file_desc, f->buf, f->buf_size);
        if(res < 0) return -1;

        // cas où res < f->buf_size
        for (unsigned int i = res; i < f->buf_size; i++) {
            // on decale tout à gauche
            ((char*)f->buf)[i-res] = ((char*)f->buf)[i];
        }
        f->buf_size -= res;
    }
    return 0;
}


int iobuf_vprintf(IOBUF_FILE *f, const char *format, va_list args) {
    if (f->mode != 'W') return -1;

    int caractere_ecris = 0;

    // fwrite octet par octet
    int i = 0;
    while (format[i]) {
        if (format[i] == '%') {
            char type = format[i+1];
            if (type == 'c') {
                char c = (char)va_arg(args, int);
                caractere_ecris += iobuf_write(&c, 1, 1, f);
            }
            else if (type == 'd') {
                int d = va_arg(args, int);
                char c;

                // si d négatif
                if (d < 0) {
                    c = '-';
                    caractere_ecris += iobuf_write(&c, 1, 1, f);
                    d = -d;
                }

                // puissance de 10 majorant d
                unsigned int puissance = 1;
                while (puissance <= d) puissance *= 10;

                // d chiffre par chiffre
                do {
                    if (puissance != 1) puissance /= 10;
                    c = (char) (d / puissance + ((int)'0'));
                    caractere_ecris += iobuf_write(&c, 1, 1, f);
                    d = d % puissance;
                } while (puissance != 1);
            }
            else if (type == 's') {
                char* s = va_arg(args, char*);

                // taille de s
                unsigned int len = 0;
                while (s[len] != 0) len++;

                caractere_ecris += iobuf_write(s, 1, len, f);
            }
            else {
                caractere_ecris += iobuf_write(&format[i], 1, 1, f);
                i--;
            }
            i++;
        }
        else {
            caractere_ecris += iobuf_write(&format[i], 1, 1, f);
        }
        i++;
    }

    return caractere_ecris;
}

int iobuf_fprintf(IOBUF_FILE *f, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = iobuf_vprintf(f, format, args);
    va_end(args);
    return ret;
}

/* directly in stdout */
int iobuf_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = iobuf_vprintf(stdout, format, args);
    va_end(args);
    return ret;
}

int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...) {
    if (f == NULL || format == NULL) return -1;

    char c;

    char* car;
    char* chaine;
    int* entier;

    int nombre_arguments_lus = 0;

    va_list parametres;

    va_start(parametres, *format);

    int i = 0;
    while(format[i] != '\0'){
        while (format[i] == ' ') i++;
        if(format[i] != '%'){
            va_end(parametres);
            return nombre_arguments_lus;
        } 
        else{
            i++;
            c = format[i];
            switch(c){
                case 'c':{
                    char car_apres;
                    car = va_arg(parametres, char*);
                    nombre_arguments_lus += iobuf_read(car, sizeof(char), 1, f);
                    // Apres le caractère, il doit y avoir un espace ou '\0
                    if (iobuf_read(&car_apres, sizeof(char), 1, f) == 1 && car_apres != ' '){
                        nombre_arguments_lus -= 1;
                        va_end(parametres);
                        return nombre_arguments_lus;
                    }
                    break;
                }

                case 's':{
                    chaine = va_arg(parametres, char*);
                    int k = 0;
                    char temp;

                    while (iobuf_read(&temp, sizeof(char), 1, f) == 1 && temp != ' ' && temp != '\0') {
                        chaine[k] = temp;
                        k++;
                    }
                    // ..................
                    nombre_arguments_lus += 1;
                    break;
                }
                
                case 'd' :
                    entier = va_arg(parametres, int*);
                    nombre_arguments_lus += iobuf_read(entier, sizeof(int), 1, f);
                    break;
                
                default :
                    va_end(parametres);
                    return nombre_arguments_lus;
                    break;
            }
            i++;
        }
    }
    va_end(parametres);
    return nombre_arguments_lus;
}
