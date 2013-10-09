CC = gcc
CFLAGS = -I../include
AR = ar

TARGET=bplus_tree

all: lib$(TARGET).a

bplus_tree.o: bplus_tree.c
bplus_node.o: bplus_node.c
bplus_leaf.o: bplus_leaf.c

bplus_insert.o: bplus_insert.c
bplus_remove.o: bplus_remove.c
bplus_search.o: bplus_search.c
bplus_foreach.o: bplus_foreach.c

bplus_iterator.o: bplus_iterator.c
bplus_rebalance.o: bplus_rebalance.c


lib$(TARGET).a: 	bplus_tree.o bplus_node.o bplus_leaf.o \
					bplus_insert.o bplus_remove.o bplus_search.o \
					bplus_foreach.o bplus_iterator.o bplus_rebalance.o
	$(AR) -cq $@ $+

clean:
	rm -rf *.o lib$(TARGET).a

