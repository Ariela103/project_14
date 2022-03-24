
	

all:   data.h  globalVariables.c encode.c exportFiles.c  errors.c utils.c helpers.c parse.c operations.c tables.c memory.c firstRun.c secondRun.c compiler.c
	gcc -ansi -Wall -pedantic -g  globalVariables.c encode.c  exportFiles.c errors.c utils.c helpers.c parse.c operations.c tables.c  memory.c secondRun.c firstRun.c compiler.c -o compiler -lm
