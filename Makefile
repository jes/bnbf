#Makefile for bnbf
#James Stanley 2010

CFLAGS=-g -Wall
LDFLAGS=-lgmp
OBJS=bnbf.o options.o program.o memory.o

bnbf: $(OBJS)

clean:
	-rm bnbf $(OBJS)
