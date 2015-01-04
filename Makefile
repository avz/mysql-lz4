CC?=cc
LD=$(CC)

PROJECT=mysql-lz4

OBJS=main.o
VPATH=src

CFLAGS?=-O2

build: mysql-lz4.so

mysql-lz4.so: $(OBJS) lz4.o
	$(CC) -lc -shared -o mysql-lz4.so $(OBJS) lz4.o

lz4.o: deps/lz4-r127/lib/lz4.c
	$(CC) -c -fPIC -std=c90 -g -D_DEFAULT_SOURCE $(CFLAGS) deps/lz4-r127/lib/lz4.c

.c.o:
	$(CC) -c -fPIC -std=c90 -g -Wall -Wconversion -D_DEFAULT_SOURCE $(CFLAGS) src/$*.c

clean:
	rm -f *.o *.so

install: build
	install mysql-lz4.so "`mysql_config --plugindir`"
