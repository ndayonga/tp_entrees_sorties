#include <unistd.h>
#include <stdlib.h>
#include "stdes.h"

int main (int argc, char **argv)
{
  IOBUF_FILE *f1, *f2;
  int n;
  if (argc != 3) exit (-1);

  f1 = iobuf_open (argv[1], 'W');
  if (f1 == NULL) exit (-1);

  n = iobuf_fprintf (f1, "Test char: '%c' and string: '%s' with normal number 12\n", 'a', "bonjour");
  iobuf_fprintf (f1, "Test number: '%d' char written on the above line\n", n);

  for (n=-11;n<11;n++)
    iobuf_fprintf(f1, "%d ", n);
  iobuf_fprintf (f1, "\n", n);
  
  iobuf_close (f1);

  {
    char c;
    int  d;
    char my_word[128];
    f2 = iobuf_open (argv[2], 'R');
    iobuf_fscanf (f2, "  test  %c end %s", &c, my_word);
    iobuf_printf ("Read '%c' and '%s' from %s\n", c, my_word, argv[2]);
    iobuf_fscanf (f2, "end %d %s", &d, my_word);
    iobuf_printf ("Now read '%d' and '%s' from %s\n", d, my_word, argv[2]);

    iobuf_close (f2);
  }

  iobuf_flush(stdout);
  return 0;
}
