
#define BINARY_WORD_SIZE 15
#define MEMORY_START 100
#define REGS_SIZE 8
#define OP_SIZE 16
#define RAM_MEMORY_SIZE 4096
#define MAX_LABEL_LEN 31
#define MAX_LINE_LEN 81
#define MAX_FILENAME_LEN 256

/* Hash Table Hashsize (Result is currently 64)  */
#define HASHSIZE ((RAM_MEMORY_SIZE / (MAX_LABEL_LEN + 1)) / 4)

/* Masks for creation of to binary word memory image */
#define A 0x004
#define R 0x002
#define E 0x001

#define IMMEDIATE_ADDR 1
#define DIRECT_ADDR 2
#define INDIRECT_ADDR 4
#define REGISTER_DIRECT_ADDR 8

#define R0 0x0
#define R1 0x1
#define R2 0x2
#define R3 0x3
#define R4 0x4
#define R5 0x5
#define R6 0x6
#define R7 0x7

#define DATA ".data"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERNAL ".extern"
/* convert the instructions to numbers*/
#define _TYPE_DATA 11
#define _TYPE_STRING 22
#define _TYPE_ENTRY 33
#define _TYPE_EXTERNAL 44
#define _TYPE_CODE 55