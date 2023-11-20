#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include "stdes.h"

#define MEMORY_SIZE 1280000 /* 1,3 Mo */

/*#define DEBUG*/

char MEMORY[MEMORY_SIZE];


void init_mem(void)
{
  int i;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);

  for (i=0; i<MEMORY_SIZE; )
    MEMORY[i++] = (char) (rand () % 95 + 0x20); /* printable characters */
}

#define RAND_NUM 1 + rand () % 128 + 100 * (rand () % 3) + (4000 * ((rand () % 4)/4) * (rand() & 1))

void mem_write (IOBUF_FILE * f)
{
  int count = 0;
  int num;
  int lr;
  iobuf_printf("Writing file...\n");

  while (count < MEMORY_SIZE) {
    num = RAND_NUM ;

    if (count + num > MEMORY_SIZE)
      num = MEMORY_SIZE - count;

#ifdef DEBUG
    iobuf_printf ("Writting %d / %d octets\n", num, count);
#endif

    lr = iobuf_write (MEMORY + count, 1, num, f);
#ifdef DEBUG
    if (lr != num)
      iobuf_printf (" -- Wrote only %d octets\n", lr);
#endif
    count += lr;
  }
  iobuf_printf ("Done\n");
  iobuf_flush (stdout);
}

void mem_read (IOBUF_FILE* f, char* buff)
{
  int count = 0;
  int num;
  int lr;
  iobuf_printf ("Reading file...\n");
  do {
    num = RAND_NUM ;
#ifdef DEBUG
    iobuf_printf ("Reading %d \\ %d octets\n", num, count);
#endif
    lr = iobuf_read (buff, 1, num, f);
#ifdef DEBUG
    if (lr != num)
      iobuf_printf (" -- Read only %d octets\n", lr);
#endif
    buff += lr;
    count += lr;
    assert (count <= MEMORY_SIZE);
  } while (lr);
  iobuf_printf ("Done\n");
  iobuf_flush (stdout);
}

void mem_compare (char* ref, char *buff)
{
  int i;
  iobuf_printf ("Comparing memories...\n");
  for (i=0; i<MEMORY_SIZE; i++) {
    if (ref[i] != buff[i]) {
      iobuf_fprintf(stderr, "ERROR %c != %c at index %d\n", 
          ref[i], buff[i], i);
    }
  }
  iobuf_printf("Done\n");
  iobuf_flush(stdout);
}


int main(int argc, char *argv[])
{
  IOBUF_FILE *f;
  char* filename = "rand-file.txt";

  init_mem ();

  f = iobuf_open (filename, 'E');
  if (f == NULL)
    exit (-1);
  mem_write (f);
  iobuf_close (f);

  {
    char *buffer = malloc (sizeof(char) * MEMORY_SIZE);
    f = iobuf_open (filename, 'L');
    if (f == NULL)
      exit (-1);
    mem_read (f, buffer);
    mem_compare (MEMORY, buffer);
    free (buffer);
    iobuf_close (f);
  }

  return 0;
}