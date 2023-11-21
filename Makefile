CC=gcc
CFLAGS=-Wall -g
LDLIBS=-lm

TEST=test test-simple test-rand test-format test-bigread test-bigwrite
PROG=eval-write


all: $(PROG) $(TEST)

test-all: $(TEST)
	echo "Je suis un fichier de test\nAvec plusieurs lignes\n;)" > toto1
	./test toto1 toto2
	./test-simple toto3
	./test-format totoecr toformat.txt
	./test-rand
	./test-bigread to1 to2
	./test-bigwrite to3
	echo "#########     TESTS OK     #########"

eval-all: $(PROG)
	echo "OK"

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

test: test.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)
	
test-%: test-%.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)

eval-write: eval-write.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	$(RM) $(PROG) $(TEST) *.o to* rand-file.txt
