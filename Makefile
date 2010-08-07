#Makefile for bnbf
#James Stanley 2010

#Use GMP (fast) bigint:
GMP_CFLAG=-DUSE_GMP
GMP_LDFLAG=-lgmp

#Use built-in (slow) bigint:
#GMP_CFLAG=-DNO_GMP
#GMP_LDFLAG=

###########################

CFLAGS=-g -Wall -Wextra -O3 $(GMP_CFLAG)
LDFLAGS=$(GMP_LDFLAG)
PREFIX?=/usr
OBJS=src/bnbf.o src/options.o src/program.o src/memory.o src/bigint.o

src/bnbf: $(OBJS)

clean:
	-rm -f src/bnbf $(OBJS)
.PHONY: clean

install:
	install -m 0755 src/bnbf $(DESTDIR)$(PREFIX)/bin
	install -m 0644 bnbf.1 $(DESTDIR)$(PREFIX)/share/man/man1
.PHONY: install

test:
	cd test; ./test.sh
.PHONY: test
