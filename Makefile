CC=gcc
CFLAGS=-Wall -g -O2
LDLIBS=

all: test

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

test: test.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)
	
test-format: test-format.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)
	
test-rand: test-rand.o stdes.o
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	$(RM) test test-format test-rand *.o
