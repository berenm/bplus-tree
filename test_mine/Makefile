CC = gcc
CFLAGS = -I../include -I../src -g
LIBS = ../src/libbplus_tree.a

all: test1

test1.o: test1.c

test1: test1.o
	$(CC) $+ $(LIBS) -o $@

clean:
	rm -rf *.o test1

