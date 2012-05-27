all: build/bplus_tree_test

build: Makefile
	install -d build

build/bplus_tree_test: Makefile build src/*.[ch] include/*.h
	clang -o build/bplus_tree_test test/bplus_tree_test.c `pkg-config --cflags --libs glib-2.0` -g -ggdb -Iinclude/ -Isrc/
