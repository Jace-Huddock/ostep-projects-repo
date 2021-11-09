## MAKEFILE for Project 3: Jace Huddock

all: dataOneHundred freeThenPrint funnyValue mallocError myVector null 

clean:
	rm -f dataOneHundred freeThenPrint funnyValue mallocError myVector null
dataOneHundred: dataOneHundred.c
	gcc -o  data dataOneHundred.c -Wall -Werror
freeThenPrint:
	gcc -o free freeThenPrint.c -Wall -Werror
mallocError:
	gcc -o malloc mallocError.c -Wall -Werror	
myVector:
	gcc -o vect myVector.c -Wall -Werror 
null: null.c
	gcc -o null null.c -Wall -Werror


	
