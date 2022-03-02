#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern unsigned currentLine;
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];
extern Operation *getOperationByName(char *s);
extern Item *addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
extern Item *findOrAddSymbol(char *name, ItemType);
extern Item *findSymbol(char *name, ItemType type);
extern Item *updateSymbolAttribute(char *name, int type);
extern Item *updateSymbolAddressValue(char *name, int newValue);
Item *removeFromTable(char *name, ItemType type);
extern void increaseDataCounter(int amount);
extern void increaseInstructionCounter(int amount);
extern unsigned getDC();
extern unsigned getIC();
extern void addNumberToMemory(int number);

int parseExpandedSourceFile(FILE *fp, char *filename)
{
    int c = 0;
    int i = 0;
    char line[MAX_LINE_LEN + 1] = {0};
    ParseState state = newLine;
    while (((c = fgetc(fp)) != EOF) && globalState != collectErrors)
    {

        if (i >= MAX_LINE_LEN - 1 && c != '\n')
        {
            globalState = collectErrors;
            return yieldError(maxLineLengthExceeded);
        }

        if (!isspace(c))
            line[i++] = c;

        if (c == '\n')
        {
            state = parseSingleLine(line, state);
            memset(line, '\0', i);
            i = 0;
        }
    }

    return True;
}

int parseSingleLine(char *line, ParseState state)
{

    int n = 0;
    char *p = calloc(strlen(line + 1), sizeof(char *));
    char *token = calloc(MAX_LABEL_LEN, sizeof(char *));
    memcpy(p, line, strlen(line));
    token = strtok(p, " \t \n");
    if (state == newLine)
        state = handleState(token, p, state);
    printf("\n\n\t\t~ Currently parsing: ~\t\t\n\"%s\" (Line number 0%d)\n", line, currentLine);
    while (token != NULL)
    {

        switch (state)
        {
        case lineParsedSuccessfully:
            state = True;
            break;
        case parseLabel:
        {
            state = handleLabel(token, strtok(NULL, " \t \n"), line);
            break;
        }

        case parseInstruction:
        {
            state = handleInstruction(getInstructionType(token), token, strtok(NULL, " \t \n"));
            break;
        }

        case parseOperation:
        {
            state = handleOperation(getOperationByName(token), token + n, line);
            break;
        }

        case parseDataVariables:
        {
            break;
        }

        case parseStringVariables:
        {
            break;
        }

        case Err:
        {
            globalState = collectErrors;
            state = Err;
            break;
        }

        case skipLine:
            state = True;

        default:
            break;
        }
        token = strtok(NULL, " \t \n");
    }

    currentLine++;
    free(p);
    free(token);
    return state;
}

int handleState(char *token, char *line, ParseState state)
{
    switch (state)
    {
    case skipLine:
        break;
    case newLine:
    {
        if (token[0] == ';')
            return skipLine;

        else if (isLabel(token))
            return parseLabel;

        else if (token[0] == '.' && getInstructionType(token))
            return parseInstruction;

        else if (isOperation(token))
            return parseOperation;
        else
            return Err;
        break;
    }

    default:
        break;
    }

    return True;
}

int handleOperation(Operation *op, char *operands, char *line)
{
    /*     printf("inside handleOperation\noperands:%s\n", operands);
     */
    return 1;
}

int handleInstruction(int type, char *firstToken, char *nextTokens)
{
    /*     printf("instructionType:%s firstToken:%s nextToken:%s\n", getInstructionNameByType(type), firstToken, nextTokens);
     */
    if (isInstruction(firstToken))
    {
        /*   printf("FIRST TOKEN IS INSTRUCTION!!!\n");
         */
        if ((type == _TYPE_ENTRY || type == _TYPE_EXTERNAL) && strlen(nextTokens) <= MAX_LABEL_LEN)
        {

            char symbolName[MAX_LABEL_LEN] = {0};
            sscanf(nextTokens, "%s", symbolName);
            nextTokens = strtok(NULL, " \t \n");
            if (nextTokens)
                return yieldError(illegalApearenceOfExtraCharactersOnLine);

            if (type == _TYPE_ENTRY)
                return addSymbol(symbolName, 0, 0, 0, 1, 0) ? lineParsedSuccessfully : Err;
            if (type == _TYPE_EXTERNAL)
                return addSymbol(symbolName, 0, 0, 0, 0, 1) ? lineParsedSuccessfully : Err;
        }

        else if (type == _TYPE_DATA)
            return handleInstructionDataArgs(nextTokens);

        else if (type == _TYPE_STRING)
            return handleInstructionStringArgs(nextTokens);
    }
    else
    {

        if (!updateSymbolAttribute(firstToken, type) || !updateSymbolAddressValue(firstToken, getDC()))
            return Err;

        return type == _TYPE_DATA ? handleInstructionDataArgs(nextTokens) : handleInstructionStringArgs(nextTokens);
    }

    return yieldError(undefinedOperation);
}

int handleLabel(char *labelName, char *nextToken, char *line)
{

    /*cleaning the label token from the last : character as we should do before we try to add it to the symbol table.     */
    labelName[strlen(labelName) - 1] = '\0';

    /*  printf("labelName:%s nextToken:%s line:%s\n", labelName, nextToken, line);
     */
    if (nextToken && (!strcmp(EXTERNAL, nextToken) || !strcmp(ENTRY, nextToken)))
        return handleInstruction(getInstructionType(nextToken), nextToken, strtok(NULL, " \t \n"));

    if (addSymbol(labelName, 0, 0, 0, 0, 0) != NULL)
    {
        if (nextToken[0] == '.')
        {
            int instruction = getInstructionType(nextToken);
            if (instruction)
                return handleInstruction(instruction, labelName, strtok(NULL, " \t \n"));

            else
                return yieldError(undefinedInstruction);
        }

        else if (getOpIndex(nextToken) != -1)
        {
            /*
                int opIndex = -1;
    int memoryAddress = -1;
                        Word *new = calloc(1, sizeof(Word *));
                        new->value->hex = generateFirstWordEncodedHex(getOperationByIndex(opIndex));
                        memoryAddress = writeToMemory(new, Code);
                        return handleOperation(getOperationByIndex(opIndex), nextToken, line); */
        }
        else
            return yieldError(illegalLabelUseExpectedOperationOrInstruction);
    }

    return Err;
}

int isOperation(char *s)
{
    int opIndex = -1;
    opIndex = getOpIndex(s);

    /* printf("inside is operation op index:%d\n", opIndex);
     */
    return opIndex != -1 ? opIndex : -1;
}

int isLabel(char *s)
{
    int len = strlen(s);
    if (len <= 1)
        return yieldError(illegalLabelNameLength);

    return s[len - 1] == ':' ? True : False;
}

int getInstructionType(char *s)
{
    if (!strcmp(s, DATA))
        return _TYPE_DATA;
    if (!strcmp(s, STRING))
        return _TYPE_STRING;
    if (!strcmp(s, ENTRY))
        return _TYPE_ENTRY;
    if (!strcmp(s, EXTERNAL))
        return _TYPE_EXTERNAL;
    return False;
}

char *getInstructionName(char *s)
{
    if (!strcmp(s, DATA))
        return DATA;
    if (!strcmp(s, STRING))
        return STRING;
    if (!strcmp(s, ENTRY))
        return ENTRY;
    if (!strcmp(s, EXTERNAL))
        return EXTERNAL;
    return 0;
}

Bool isInstruction(char *s)
{
    return (!strcmp(s, DATA) || !strcmp(s, STRING) || !strcmp(s, ENTRY) || !strcmp(s, EXTERNAL)) ? True : False;
}

char *getInstructionNameByType(int type)
{
    switch (type)
    {
    case _TYPE_DATA:
        return "DATA INSTRUCTION";

    case _TYPE_STRING:
        return "STRING INSTRUCTION";

    case _TYPE_ENTRY:
        return "ENTRY INSTRUCTION";

    case _TYPE_EXTERNAL:
        return "EXTERNAL INSTRUCTION";

    default:
        break;
    }

    return "NOT AN INSTRUCTION!";
}

int handleInstructionDataArgs(char *token)
{
    int number = 0;
    int counter = 0;
    Bool commaState = True;
    while (token)
    {
        if (token[0] == ',')
        {
            if (commaState)
                return yieldError(wrongInstructionSyntaxExtraCommas);
            else
                commaState = True;
        }

        else if (!isdigit(token[0]))
            return yieldError(illegalApearenceOfCharactersOnLine);

        else if (commaState)
        {
            sscanf(token, "%d", &number);
            if (globalState == secondRun)
                addNumberToMemory(number);

            if (token[strlen(token) - 1] != ',')
                commaState = True;
            else
                commaState = False;

            counter++;
        }
        else if (!commaState)
            return yieldError(expectedSingleCommaCharacter);

        token = strtok(NULL, " \t \n");
    }

    if (globalState != secondRun)
        increaseDataCounter(counter);

    return lineParsedSuccessfully;
}

int handleInstructionStringArgs(char *token)
{

    /* printf("inside handle instruction string args, token:%s\n", token);
     */

    if (!(token[0] == '\"') || !(token[strlen(token) - 1] == '\"'))
        return yieldError(expectedQuotes);

    if (globalState != secondRun)
        increaseDataCounter((int)(strlen(token) - 2));

    else
    {
        int i = 1;
        while (i < strlen(token) - 2)
        {
            addNumberToMemory((int)token[i]);
            i++;
        }
        addNumberToMemory((int)'\0');
    }

    return lineParsedSuccessfully;
}

/*
                  IC -> 10
DC = Data Counter IC = Instruction counter
1 - comment -> skip
2 -> empty -> skip
3 -> error in line -> print specific error, change globalState to collect error, skip line
4 -> label or instruction -> check next token -> ? x: .data 3,3,5,67,7 DC
A) label is data/string/entry/external -> addSymbol(x, DC)
-> if(globalState == firstRun) upadteDataCounter(someWordAmount)
-> if(globalState == secondRun)writeToMemory(value=3,Data)
B) label is operation -> addSymbol(labelName, IC) ->
-> if(globalState == firstRun) upadteOperationCounter(someWordAmount)
-> if(globalState == secondRun) writeToMemory(word,Code) for each word of operation
*/
