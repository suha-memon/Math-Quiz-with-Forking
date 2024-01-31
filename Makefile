# Makefile for project

CC		= gcc

CFLAGS		= -g  -Wall -std=c99

CLIBS		= -lm 

all: problem1

project: ParentProcess.c
	$(CC) $(CFLAGS) -o ParentProcess ParentProcess.c
	$(CC) $(CFLAGS) -o ChildProcess ChildProcess.c

run_project: project
	./ParentProcess

clean:
	rm -f *~ *.o a.out core project