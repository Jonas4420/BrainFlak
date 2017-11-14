.PHONY: all clean mrproper

CC      = gcc
CFLAGS  = -O3 -Wall -pedantic
LDFLAGS =

PROG  = brainflak
SRC   = main.o brainflak.o stack.o

all: $(PROG)

$(PROG): $(SRC)
	$(CC) $(LDFLAGS) -o $(PROG) $^

brainflak.o: brainflak.c brainflak.h
	$(CC) $(CFLAGS) -c brainflak.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

clean:
	rm -rf $(PROG).dSYM
	rm -rf *.o

mrproper: clean
	rm -rf $(PROG)
