CC = gcc
CFLAGS = -g -Wall -Og

main: main.o myheap.o

clean:
	-/bin/rm -rf main main.o myheap.o
tidy: clean
	-/bin/rm -rf *~ .*~

main.o: main.c myheap.h
myheap.o: myheap.c myheap.h
