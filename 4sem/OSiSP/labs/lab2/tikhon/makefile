CC = gcc
CFLAGS = -W -Wall -Wextra -std=c11 -Wno-unused-parameter
CHILD_PATH = /home/tikhon/Документы/Vscode/"Lab2 osisp"/child.exe

.PHONY: all clean

all: parent child

parent: parent.c makefile
	$(CC) $(CFLAGS) parent.c -o parent.exe

child: child.c makefile
	$(CC) $(CFLAGS) child.c -o child.exe

clean:
	rm -f parent.exe child.exe

run: parent child
	export CHILD_PATH=$(CHILD_PATH) && ./parent.exe	env.txt