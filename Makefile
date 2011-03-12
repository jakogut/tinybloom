CC = gcc
AR = ar
CFLAGS = -O3 -Wall -Werror

LIBNAME_STATIC = libtinybloom.a
LIBNAME_SHARED = libtinybloom.so

all: tinybloom_static

tinybloom_static: tinybloom.o
	ar rcs $(LIBNAME_STATIC) tinybloom.o

test: tinybloom_static test.o
	$(CC) test.o -o test -ltinybloom

install:
	cp -f $(LIBNAME_STATIC) /usr/lib/
	cp -f tinybloom.h /usr/include/

uninstall: 
	rm -f /usr/lib/libtinybloom*
	rm -f /usr/include/tinybloom*

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.a *.so test

.PHONY: clean
.SILENT: clean
