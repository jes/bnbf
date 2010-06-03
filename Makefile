#Makefile for bnbf
#James Stanley 2010

CFLAGS=-g -Wall
LDFLAGS=-lgmp
OBJS=bnbf.c options.c

bnbf: $(OBJS)
