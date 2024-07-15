CC = gcc
CFLAGS = -W -Wall -Wextra -std=c11
.PHONY: clean

all: parent child

parent: parent.c utils.h
	$(CC) $(CFLAGS) parent.c -o parent

child: child.c utils.h
	$(CC) $(CFLAGS) child.c -o child

run:
	./parent

clean: 
	rm parent child