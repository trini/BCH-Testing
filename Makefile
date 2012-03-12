CC = gcc
CFLAGS = -Wall -O3 -g -Iinclude

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

all: bch.o
	$(CC) -o bch bch.o

clean:
	rm -f bch *.o
