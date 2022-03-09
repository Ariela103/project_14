/*

Symbol table is fully updated with all of the symbols with correct
values (.data/.string/.entry arguments address is updated as well to start after last
operation address)


 */
#include "data.h"
/* from variables.c (global Variables) */
extern State globalState;
extern unsigned currentLine;
extern const char *regs[REGS_SIZE];

/* from firstRun.c */
extern void parseSingleLine(char *line);
extern ParseState handleFirstToken(char *token, char *line, ParseState state);
extern Bool parseOperands(char *src, char comma, char *des, Operation *op, AddrMethodsOptions active[2]);

/* from table.c: */
extern int getSymbolBaseAddress(char *name);
extern int getSymbolOffset(char *name);

/* from operation.c */
extern Operation operations[OP_SIZE];
extern Operation *getOperationByName(char *s);

/* from memory.c */
extern unsigned getDC();
extern unsigned getIC();
extern void writeIntegerIntoDataMemoryBinaryImg(int number);

int secondRunParseSource(FILE *fp, char *filename)
{
    int c = 0;
    int i = 0;
    char line[MAX_LINE_LEN + 1] = {0};
    currentLine = 1;

    printf("Second Run:\n");
    while (((c = fgetc(fp)) != EOF))
    {

        if (c == '\n')
        {
            parseSingleLine(line);
            memset(line, 0, MAX_LINE_LEN);
            i = 0;
        }

        else if (isspace(c))
            line[i++] = ' ';

        else
        {
            if (isprint(c))
                line[i++] = c;
        }
    }

    printf("line: %s length: %d \n", line, (int)strlen(line));
    if (i > 0)
    {
        parseSingleLine(line);
        memset(line, 0, i);
    }

    return True;
}

Bool writeOperationBinary(char *operationName, char *line)
{
    Operation *p = getOperationByName(operationName);
    char firstOperand[MAX_LABEL_LEN] = {0}, secondOperand[MAX_LABEL_LEN] = {0};
    char comma = 0;
    int nTotal = 0, nFirst = 0;
    AddrMethodsOptions active[2] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
    line = operationName + strlen(operationName) + 1;

    sscanf(line, "%s%n%c%s%n", firstOperand, &nFirst, &comma, secondOperand, &nTotal);

    if (secondOperand[0] == 0 && firstOperand[0] != 0)
    {
        if (!strchr(firstOperand, ','))
        {
            memcpy(secondOperand, firstOperand, nFirst);
            firstOperand[0] = '\0';
        }
        else
        {
            memcpy(secondOperand, strrchr(firstOperand, ','), strlen(firstOperand));
            firstOperand[strlen(firstOperand) - strlen(secondOperand)] = 0;
        }
    }

    if (parseOperands(firstOperand, comma, secondOperand, p, active))
    {
        /*  printf("line 183, handle Operation\n");
         printf("active:\nSRC: direct:%u index:%u immediate:%u reg:%u\n", active[0].direct, active[0].index, active[0].immediate, active[0].reg);
         printf("DES: direct:%u index:%u immediate:%u reg:%u\n", active[1].direct, active[1].index, active[1].immediate, active[1].reg); */

        unsigned base = 0, offset = 0;
        if (active[0].direct || active[0].index)
        {

            base = getSymbolBaseAddress(firstOperand);
            offset = getSymbolOffset(secondOperand);
        }
        if (active[1].direct || active[1].index)
        {
            base = getSymbolBaseAddress(secondOperand);
            offset = getSymbolOffset(secondOperand);
        }

        if (base == -1 || offset == -1)
        {
            yieldError(labelNotExist);
        }
    }

    return True;
}
