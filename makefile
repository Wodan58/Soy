#
#   module  : makefile
#   version : 1.10
#   date    : 12/16/24
#
.POSIX:
.SUFFIXES:
.SUFFIXES: .c .o

PROG   = fib
# Use CC environment variable
# CC   = gcc
CFLAGS = -O3 -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter
HDRS   = globals.h
OBJS   = $(PROG).o main.o exeterm.o writ.o print.o prog.o save.o

$(PROG): prep $(OBJS)
	$(CC) -o$@ $(OBJS) -lm -lgc

$(OBJS): $(HDRS)

prep:
	sh prim.sh .
	sh defs.sh .
	sh deps.sh .

.c.o:
	$(CC) -o$@ $(CFLAGS) -c $<

clean:
	rm -f *.o
