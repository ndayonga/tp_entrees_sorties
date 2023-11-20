CC=gcc
CFLAGS=-Wall -g -O1
LDLIBS=

TEST=test test-simple test-rand test-format
PROG=$(TEST) eval-libc eval-syscall


all: $(PROG)

test-all: $(TEST)
	echo "Je suis un fichier de test\nAvec plusieurs lignes\n;)" > toto1
	./test toto1 toto2
	./test-simple toto3
	echo "#########     TESTS OK     #########"

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

test: test.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)
	
test-%: test-%.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	$(RM) $(PROG) *.o to*
