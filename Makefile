##Makefile for Project 2: Unix Shell, Jace Huddock

all: wish

clean:
	rm -f wish
wish:
	gcc wish.c -Wall -Werror

