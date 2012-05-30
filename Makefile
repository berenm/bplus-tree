all: build/bplus_tree_test

build: Makefile
	install -d build

coverage: Makefile
	install -d coverage

test: Makefile build/bplus_tree_test coverage/bplus_tree_test
	./build/bplus_tree_test
	cd coverage && ./bplus_tree_test 1>/dev/null 2>/dev/null && lcov --capture --directory . --output bplus_tree_test.info && genhtml bplus_tree_test.info

coverage/bplus_tree_test: Makefile coverage src/*.[ch] include/*.h
	cd coverage && gcc -std=c99 -fprofile-arcs -ftest-coverage -o bplus_tree_test ../test/bplus_tree_test.c `pkg-config --cflags --libs glib-2.0` -g -ggdb -I../include/ -I../src/

build/bplus_tree_test: Makefile build src/*.[ch] include/*.h
	cd build && clang -std=c99 -o bplus_tree_test ../test/bplus_tree_test.c `pkg-config --cflags --libs glib-2.0` -g -ggdb -I../include/ -I../src/
