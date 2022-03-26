#include "data.h"
#include <stdarg.h>

extern Bool yieldError(Error err);
extern Bool yieldWarning(Warning err);
extern char *trimFromLeft(char *s);
extern int countConsecutiveCommas(char *s);
extern int countLengthOfNonDigitToken(char *s);
static void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;
extern FILE *getSourceFilePointer();
static void (*resetCurrentLineCounter)() = &resetCurrentLineNumber;

Bool handleSingleLine(char *line);

/* @ Function: countAndVerifyDataArguments
   @ Arguments: the function get char * line which is the current line that we are about to parse the data arguments from.
   @ Description: The function extracts the argument string of the .data instruction, than the function analyses\ parses the string.
   If the function encounter errors or illegal arguments, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
   While the function parsing the arguments, ir also counts the number of .data elements that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .data instruction will take.
*/
Bool countAndVerifyDataArguments(char *line)
{

    Bool isValid = True;
    Bool minusOrPlusFlag = False;
    char args[MAX_LINE_LEN + 1] = {0};
    int size = 0, num = 0, n = 0, commasCounter = 0, i = 0, len = 0, skip = 0;
    char c = 0, *p;
    /* we make the pointer p to point on the position of the first character coming sfter the .data
     instruction within the full line, so that p will point on the begining of the arguments string*/
    p = (strstr(line, DATA) + strlen(DATA));
    len = strlen(p);
    /*copies the string of arguments pointer by p into the args local string we will use for parsing*/
    memcpy(args, p, len);
    if (!strlen(args))
        return yieldWarning(emptyDataDeclaretion);
    p = args;
    p = trimFromLeft(p);
    i = len - strlen(p);

    if (*p == ',')
    {
        isValid = yieldError(illegalApearenceOfCommaBeforeFirstParameter);
        p++;
        i++;
    }

    while (p && i < len)
    {

        if (!isspace(p[0]))
        {
            i = len - strlen(p);
            if (!isdigit(p[0]))
            {

                if (*p == '-' || *p == '+')
                {
                    if (!minusOrPlusFlag)
                    {
                        if (isspace(p[1]))
                        {
                            isValid = yieldError(afterPlusOrMinusSignThereMustBeANumber);
                            minusOrPlusFlag = False;
                        }
                        else
                            minusOrPlusFlag = True;
                    }
                    else
                    {
                        isValid = yieldError(expectedNumber);
                        minusOrPlusFlag = False;
                    }
                    skip = 1;
                }
                else if (*p == ',')
                {
                    skip = countConsecutiveCommas(p);
                    commasCounter += skip;
                }
                else
                {
                    isValid = yieldError(expectedNumber);
                    skip = countLengthOfNonDigitToken(p);
                    size++;
                }
            }
            else
            {
                if (commasCounter < 1 && size > 0)
                {
                    isValid = yieldError(wrongInstructionSyntaxMissinCommas);
                    commasCounter = size;
                }

                else if (commasCounter > size)
                {
                    isValid = yieldError(wrongInstructionSyntaxExtraCommas);
                    commasCounter = size;
                }
                else if (size > commasCounter)
                {
                    isValid = yieldError(wrongInstructionSyntaxMissinCommas);
                    commasCounter = size;
                }

                i = len - strlen(p);
                sscanf(&args[i], "%d%n%c", &num, &n, &c);
                if (c && c != ',' && !isspace(c) && c != '.')
                {
                    isValid = yieldError(illegalApearenceOfCharactersOnLine);
                    n++;
                    p++;
                    i++;
                }

                else if (c == '.')
                {
                    isValid = yieldError(wrongArgumentTypeNotAnInteger);
                    p += n + 1;
                    i += n + 1;
                    sscanf(&args[i], "%d%n", &num, &n);
                }
                size++;
                minusOrPlusFlag = False;
                skip = n > 0 ? n : 1;
                c = n = num = 0;
            }
        }
        else
            skip = 1;

        p += skip;
        i += skip;
    }

    if (commasCounter > (size - 1))
        isValid = yieldError(illegalApearenceOfCommaAfterLastParameter);

    if (isValid)
        increaseDataCounter(size);

    return isValid;
}

Bool countAndVerifyOperandSyntax(char *args, char first[MAX_LINE_LEN], char second[MAX_LINE_LEN])
{
    char *s1 = args, *s2;
    Bool parsedOperands = False;
    int counter = 0;
    int commasCounter = 0;
    Bool areOperandsLegal = True;
    s1 = trimFromLeft(args);
    if (*s1 == ',')
    {
        while (*s1 && (*s1 == ',' || isspace(*s1)))
            s1++;

        if (*s1)
            areOperandsLegal = yieldError(illegalApearenceOfCommaBeforeFirstParameter);
        else
            areOperandsLegal = yieldError(illegalApearenceOfCommaAfterLastParameter);
    }

    s2 = s1;
    while (*s1 && !parsedOperands)
    {
        counter++;
        if (*s1 == ',' || isspace(*s1))
        {
            while (*s1 == ',' || isspace(*s1))
            {
                if (*s1 == ',')
                    commasCounter++;
                s1++;
            }

            if (commasCounter > 1)
                areOperandsLegal = yieldError(wrongOperationSyntaxExtraCommas);

            /*                 printf("counter -1:%d strlens2 - strlens1:%d\n", counter - 1, (int)(strlen(s2) - strlen(s1)));
             */
            strcpy(second, s1);
            strncpy(first, s2, counter - 1);
            parsedOperands = True;
        }

        s1++;
    }
    while (*s1 && *s1 != '\0' && (!isspace(*s1) && *s1 != ','))
        s1++;
    second[strlen(second) - strlen(s1)] = '\0';
    if (strlen(first) > 0 && strlen(second) < 1)
    {
        strcpy(second, first);
        memset(first, 0, strlen(first));
        if (commasCounter > 0)
            areOperandsLegal = yieldError(illegalApearenceOfCommaAfterLastParameter);
    }

    commasCounter = 0;
    counter = 0;
    while (*s1 && *s1 != '\0')
    {
        if (isprint(*s1) && !isspace(*s1) && *s1 != ',')
            counter++;
        if (*s1 == ',')
            commasCounter++;

        s1++;
    }

    if (commasCounter > 0)
        areOperandsLegal = yieldError(illegalApearenceOfCommaAfterLastParameter);

    if (counter > 0)
        areOperandsLegal = yieldError(illegalApearenceOfCharactersInTheEndOfTheLine);

    /*         printf("second:%s first:%s s1:%s\n", second, first, s1); */

    /*
            if (*s1 == ',')
            {
                yieldError(illegalApearenceOfCommaAfterLastParameter);
                second[strlen(second) - strlen(s1) - 1] = '\0';
            }
            else if (!isspace(*s1) && isprint(*s1))
                areOperandsLegal = yieldError(illegalApearenceOfCharactersInTheEndOfTheLine);
     */
    /*
            if (commasCounter > 1)
                areOperandsLegal = yieldError(wrongOperationSyntaxExtraCommas); */

    else if (commasCounter < 1 && (strlen(first) && strlen(second)))
        areOperandsLegal = yieldError(wrongOperationSyntaxMissingCommas);

    return areOperandsLegal ? True : False;
}

Bool countAndVerifyStringArguments(char *line)
{
    char *s = 0, *args;
    args = strchr(line, '\"');
    if (!args)
        return yieldWarning(emptyStringDeclatretion);

    if (args[0] != '\"')
        return yieldError(expectedQuotes);

    s = strrchr(args, '\"');
    while (*s && *s != '\0')
    {
        if (!isspace(*s) && isprint(*s) && *s != '\"')
            return yieldError(closingQuotesForStringIsMissing);
        s++;
    }

    increaseDataCounter((int)(strlen(args) - 1)); /*counts the \0 at the end of the string as well*/

    return True;
}

ParseState parseLine(char *token, char *line)
{
    State (*globalState)() = &getGlobalState;

    if (isComment(token))
        return lineParsedSuccessfully;

    if (isLabelDeclaration(token))
    {
        if (strlen(token) == 1)
            yieldError(illegalLabelDeclaration);
        else
        {
            char *next = (*globalState)() == firstRun ? strtok(NULL, " \t \n") : strtok(NULL, ", \t \n");
            if (!next)
                return yieldError(emptyLabelDecleration);

            if ((*globalState)() == firstRun)
                return handleLabel(token, next, line) ? lineParsedSuccessfully : Err;
            else
                return parseLine(next, line + strlen(token) + 1);
        }
    }

    else if (isInstruction(token))
    {
        char *next;
        int type;
        Bool isValid = True;
        printf("line:%s\n", line);

        if (!isInstructionStrict(token))
        {
            isValid = yieldError(missinSpaceAfterInstruction);
            token = getInstructionName(token);
        }
        type = getInstructionType(token);
        next = (*globalState)() == firstRun ? strtok(NULL, " \t \n") : strtok(NULL, ", \t \n");

        if (!next)
        {
            if (type == _TYPE_DATA || type == _TYPE_STRING)
                return type == _TYPE_DATA ? yieldWarning(emptyDataDeclaretion) : yieldWarning(emptyStringDeclatretion);
            else
                return type == _TYPE_ENTRY ? yieldWarning(emptyEntryDeclaretion) : yieldWarning(emptyExternalDeclaretion);
        }
        else
        {
            if ((*globalState)() == firstRun)
                return isValid && handleInstruction(type, token, next, line);
            else
            {
                if (type == _TYPE_DATA)
                    return writeDataInstruction(next) ? lineParsedSuccessfully : Err;
                else if (type == _TYPE_STRING)
                    return writeStringInstruction(next) ? lineParsedSuccessfully : Err;
                else
                    return lineParsedSuccessfully;
            }
        }
    }

    else if (isOperation(token))
    {
        char args[MAX_LINE_LEN] = {0};
        strcpy(args, (line + strlen(token)));
        return (*globalState)() == firstRun ? handleOperation(token, args) : writeOperationBinary(token, args) ? lineParsedSuccessfully
                                                                                                               : Err;
    }

    else
    {
        if (strlen(token) > 1)
            yieldError(undefinedTokenNotOperationOrInstructionOrLabel);
        else
            yieldError(illegalApearenceOfCharacterInTheBegningOfTheLine);
    }

    return Err;
}

Bool handleSingleLine(char *line)
{
    State (*globalState)() = &getGlobalState;
    ParseState state = newLine;
    char lineCopy[MAX_LINE_LEN] = {0};
    char *token;
    strcpy(lineCopy, line);
    printf("line:%s\n", line);
    token = ((*globalState)() == firstRun) ? strtok(lineCopy, " \t \n") : strtok(lineCopy, ", \t \n");
    state = parseLine(token, line);
    (*currentLineNumberPlusPlus)();
    return state == lineParsedSuccessfully
               ? True
               : False;
}

void parseAssemblyCode(FILE *src)
{
    State (*globalState)() = &getGlobalState;
    void (*setState)() = &setGlobalState;
    int c = 0, i = 0;
    char line[MAX_LINE_LEN] = {0};

    Bool isValidCode = True;
    State nextState;
    char *(*fileName)() = &getFileNamePath;

    (*resetCurrentLineCounter)();
    if ((*globalState)() == secondRun)
        printf("\n\n\nSecond Run:(%s)\n", (*fileName)());
    else if ((*globalState)() == firstRun)
        printf("\n\n\nFirst Run:(%s)\n", (*fileName)());

    while (((c = fgetc(src)) != EOF))
    {
        putchar(c);

        if (i >= MAX_LINE_LEN - 1 && !isspace(c))
        {
            isValidCode = yieldError(maxLineLengthExceeded);
            memset(line, 0, i);
            i = 0;
        }

        if (isspace(c) && i > 0)
        {
            line[i++] = ' ';
        }

        else if (isprint(c) && !isspace(c))
        {
            line[i++] = c;
        }

        if (c == '\n')
        {
            line[i++] = '\n';
            if (i > 0)
            {
                isValidCode = handleSingleLine(line) && isValidCode;
                memset(line, 0, i);
                i = 0;
            }
        }
    }

    if (i > 0)
    {
        isValidCode = handleSingleLine(line) && isValidCode;
    }

    if (!isValidCode)
        nextState = assemblyCodeFailedToCompile;
    else
        nextState = (*globalState)() == firstRun ? secondRun : exportFiles;

    (*resetCurrentLineCounter)();
    (*setState)(nextState);
}