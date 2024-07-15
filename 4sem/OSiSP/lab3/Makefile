CC = gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
.PHONY:clean

all: parent  child

run: parent child
	./parent

parent: parent.c Makefile
	$(CC) $(CFLAGS) parent.c -o parent
	
 child: child.c Makefile
	$(CC) $(CFLAGS) child.c -o child

clean:
	rm -f parent child