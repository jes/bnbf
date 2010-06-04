#Makefile for bnbf
#James Stanley 2010

CFLAGS=-g -Wall
LDFLAGS=-lgmp
OBJS=src/bnbf.o src/options.o src/program.o src/memory.o

src/bnbf: $(OBJS)

clean:
	-rm src/bnbf $(OBJS) *~
