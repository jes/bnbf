#Makefile for bnbf
#James Stanley 2010

CFLAGS=-g -Wall
LDFLAGS=
PREFIX?=/usr
OBJS=src/bnbf.o src/options.o src/program.o src/memory.o src/bigint.o

src/bnbf: $(OBJS)

clean:
	-rm -f src/bnbf $(OBJS) src/*~
.PHONY: clean

install:
	install -m 0755 src/bnbf $(DESTDIR)$(PREFIX)/bin
	install -m 0644 bnbf.1 $(DESTDIR)$(PREFIX)/share/man/man1
.PHONY: install

test:
	cd test; ./test.sh
.PHONY: test
