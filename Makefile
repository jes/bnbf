#Makefile for bnbf
#James Stanley 2010

CFLAGS=-g -Wall
LDFLAGS=-lgmp
PREFIX?=/usr
OBJS=src/bnbf.o src/options.o src/program.o src/memory.o

src/bnbf: $(OBJS)

clean:
	-rm -f src/bnbf $(OBJS) *~
.PHONY: clean

install:
	install -m 0755 src/bnbf $(DESTDIR)$(PREFIX)/bin
.PHONY: install
