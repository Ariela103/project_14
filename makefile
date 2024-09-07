
	

all:   data.h preAssembler.c sharedStates.c generateFiles.c errors.c utils.c helpers.c parse.c op.c tables.c mem.c first.c second.c main.c
	gcc -ansi -Wall -pedantic -g data.h preAssembler.c sharedStates.c generateFiles.c errors.c utils.c helpers.c parse.c op.c tables.c  mem.c second.c first.c main.c -o main -lm