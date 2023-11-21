CC=gcc
CFLAGS=-Wall -g
LDLIBS=-lm

TEST=test test-simple test-rand test-format test-bigread test-bigwrite
PROG=eval-write eval-read


all: $(PROG) $(TEST)


%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

test: test.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)
	
test-%: test-%.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)

eval-write: eval-write.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)

eval-read: eval-read.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	$(RM) $(PROG) $(TEST) *.o to* rand-file.txt
