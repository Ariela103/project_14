#include "data.h"

/* Function pointers to increment and reset the current line number */
static void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;
static void (*resetCurrentLineCounter)() = &resetCurrentLineNumber;

/**
 * countAndVerifyDataArguments
 * ---------------------------
 * This function parses and verifies the arguments in a .data instruction.
 * It checks if the syntax is valid and if the arguments are numbers.
 * It also increases the data counter if the arguments are valid.
 *
 * Parameters:
 * - line: The full line containing the .data instruction and its arguments.
 *
 * Returns:
 * - Bool: True if the arguments are valid, False otherwise.
 */
Bool countAndVerifyDataArguments(char *line)
{
    Bool isValid = True;                      /* Initially assume that the line is valid */
    int size = 0, n = 0, num = 0;             /* size tracks the number of valid arguments, n and num are for parsing */
    char c = 0;                               /* Used to detect trailing characters after numbers */
    char args[MAX_LINE_LEN + 1] = {0}, *p;    /* args stores the cleaned-up arguments from the line */
    line = strstr(line, DATA) + strlen(DATA); /* Move the pointer to after the ".data" directive */

    /* Copy the arguments after the .data directive into the args string for validation */
    strcpy(args, line);

    /* Check if the comma syntax in the arguments is valid */
    isValid = verifyCommaSyntax(args);

    /* Split the arguments by commas and whitespace */
    p = strtok(line, ", \t\n\f\r");

    /* Loop through each argument and check if it's a valid number */
    while (p != NULL)
    {
        /* Attempt to read a number from the current argument string */
        sscanf(p, "%d%n%c", &num, &n, &c); /* Parse the number, 'n' holds number of characters read, 'c' checks for trailing chars */
        if (c == '.' && n > 0)
            isValid = reportError(wrongArgumentTypeNotAnInteger); /* Error if the number contains a decimal point */
        num = atoi(p);                                            /* Convert the string to an integer */
        if (!num && *p != '0')
            isValid = reportError(expectedNumber); /* If atoi failed and it's not the number '0', it's an invalid number */

        /* Reset parsing variables and increase the size counter */
        n = num = c = 0;
        size++;
        p = strtok(NULL, ", \t\n\f\r"); /* Move to the next argument */
    }

    /* If all arguments were valid, increase the data counter by the number of arguments */
    if (isValid)
        increaseDataCounter(size);

    return isValid;
}

/**
 * verifyCommaSyntax
 * -----------------
 * This function checks the correct placement of commas in a line of arguments.
 * It ensures there are no extra or missing commas and that commas are placed correctly.
 *
 * Parameters:
 * - line: The line of arguments to check.
 *
 * Returns:
 * - Bool: True if the comma syntax is correct, False otherwise.
 */
Bool verifyCommaSyntax(char *line)
{
    int commasCounter = 0;    /* Tracks the number of commas between tokens */
    Bool insideToken = False; /* Flag to indicate if we're inside a token */
    Bool isFirstToken = True; /* Flag to indicate if we're processing the first token */
    Bool isValid = True;      /* Assume valid syntax unless proven otherwise */
    char *s = line;
    s = trimFromLeft(s); /* Remove leading whitespace */

    /* Handle commas at the beginning of the line */
    while ((*s == ',' || isspace(*s)) && *s != '\0')
    {
        if (*s == ',')
            commasCounter++; /* Count commas */
        s++;
    }
    if (!*s && commasCounter > 0)
        return reportError(wrongCommasSyntaxIllegalApearenceOfCommasInLine); /* Error if there are commas but no arguments */
    else if (*s && strlen(s) && commasCounter > 0)
        isValid = reportError(illegalApearenceOfCommaBeforeFirstParameter); /* Error if a comma appears before the first argument */

    commasCounter = 0;
    isFirstToken = True;

    /* Parse the line to ensure commas are placed correctly */
    while (s && *s != '\0')
    {
        if (insideToken)
        {
            if (commasCounter > 1)
            {
                isValid = reportError(wrongCommasSyntaxExtra); /* Too many commas between arguments */
                commasCounter = 1;
            }
            else if (commasCounter < 1 && !isFirstToken)
                isValid = reportError(wrongCommasSyntaxMissing); /* Missing commas between arguments */

            if (isFirstToken == True)
                isFirstToken = False; /* Mark that we've processed the first token */

            while (*s != '\0' && !isspace(*s) && *s != ',')
                s++; /* Skip over characters inside the token */

            if (*s == ',' || isspace(*s))
            {
                insideToken = False; /* We've reached the end of the token */
                commasCounter = 0;
                s--;
            }
        }
        else
        {
            /* Skip over commas and whitespace */
            while (*s == ',' || isspace(*s))
            {
                if (*s == ',')
                    commasCounter++;
                s++;
            }

            if (*s && (isprint(*s) && !isspace(*s)))
                insideToken = True; /* Start of the next token */
        }

        s++;
    }

    /* Report error if there's a comma after the last argument */
    if (commasCounter)
        isValid = reportError(illegalApearenceOfCommaAfterLastParameter);

    return isValid;
}

/**
 * countAndVerifyStringArguments
 * -----------------------------
 * This function checks the validity of arguments in a .string instruction.
 * It ensures the string is enclosed in quotes and increments the data counter based on the string's length.
 *
 * Parameters:
 * - line: The full line containing the .string instruction and its arguments.
 *
 * Returns:
 * - Bool: True if the arguments are valid, False otherwise.
 */
Bool countAndVerifyStringArguments(char *line)
{
    char *args, *closing = 0, *opening = 0; /* opening and closing will point to the string quotes */
    int size = 0;
    args = strstr(line, STRING) + strlen(STRING); /* Move to the arguments after the .string directive */
    args = trimFromLeft(args);                    /* Remove any leading whitespace */

    if (!*args)
        return reportError(emptyStringDeclatretion); /* Error if no string is provided */

    opening = strchr(args, '\"'); /* Find the opening quote */

    if (!opening || !*opening)
    {
        reportError(expectedQuotes);                  /* Error if the opening quote is missing */
        reportError(closingQuotesForStringIsMissing); /* Error if the closing quote is missing */
        return False;
    }
    else
    {
        closing = strrchr(args, '\"'); /* Find the closing quote */
        if (opening == closing && (opening[0] == args[0]))
            return reportError(closingQuotesForStringIsMissing); /* Error if there is only one quote */
        if (opening == closing && (opening[0] != args[0]))
            return reportError(expectedQuotes); /* Error if only the opening quote is found */
        else
        {
            size = strlen(opening) - strlen(closing); /* Calculate the size of the string */
            increaseDataCounter(size);                /* Increase the data counter based on the string length */
        }
    }

    return True;
}

/**
 * parseLine
 * ---------
 * Parses a line of assembly code, handling comments, labels, instructions, and operations.
 *
 * Parameters:
 * - token: The first token in the line (e.g., a label, instruction, or operation).
 * - line: The full line of code.
 *
 * Returns:
 * - Bool: True if the line is valid, False otherwise.
 */
Bool parseLine(char *token, char *line)
{
    State (*globalState)() = &getGlobalState; /* Get the current state of the assembler (first or second pass) */
    Bool isValid = True;                      /* Assume the line is valid initially */

    if (isComment(token))
        return True; /* Ignore comment lines */

    if (isLabelDeclaration(token))
    {
        char *next = 0;
        if (!isLabelDeclarationStrict(token))
        {
            /* Handle missing space between the label and instruction */
            char lineClone[MAX_LINE_LEN] = {0}, *rest = 0;
            strcpy(lineClone, line);                                                  /* Create a clone of the line */
            isValid = reportError(missingSpaceBetweenLabelDeclaretionAndInstruction); /* Error if no space after label */
            token = line;                                                             /* Reset token to start of the line */
            next = strchr(line, ':');
            next++;
            *next = '\0';
            rest = strchr(lineClone, ':');
            rest++;
            sprintf(line, "%s%c%s", token, ' ', rest); /* Fix the space issue */
            strncpy(lineClone, line, strlen(line));
            next = (*globalState)() == firstRun ? strtok(lineClone, " \t\n\f\r") : strtok(lineClone, ", \t\n\f\r");
            return parseLine(next, line) && False; /* Parse the fixed line */
        }
        else
        {
            next = (*globalState)() == firstRun ? strtok(NULL, " \t\n\f\r") : strtok(NULL, ", \t\n\f\r");
            if (!next)
                return reportError(emptyLabelDecleration); /* Error if label is empty */

            if ((*globalState)() == firstRun)
                return handleLabel(token, next, line) && isValid; /* Handle label on the first run */
            else
                return isValid && parseLine(next, line + strlen(token) + 1); /* Continue parsing the line */
        }
    }
    else if (isInstruction(token))
    {
        /* Handle instructions like .data and .string */
        char *next;
        int type;
        type = getInstructionType(token); /* Determine the type of instruction */
        if (!isInstructionStrict(token))
        {
            isValid = reportError(missinSpaceAfterInstruction); /* Error if there's no space after the instruction */
            token = getInstructionName(token);
        }
        next = (*globalState)() == firstRun ? strtok(NULL, " \t\n\f\r") : strtok(NULL, ", \t\n\f\r");

        if (isValid && next == NULL)
        {
            /* Handle missing arguments for instructions */
            if (type == _TYPE_DATA || type == _TYPE_STRING)
                return type == _TYPE_DATA ? reportWarning(emptyDataDeclaretion) : reportError(emptyStringDeclatretion); /* Warn or error based on instruction type */
            else
                return type == _TYPE_ENTRY ? reportWarning(emptyEntryDeclaretion) : reportWarning(emptyExternalDeclaretion);
        }
        else if (next != NULL)
        {
            if ((*globalState)() == firstRun)
                return handleInstruction(type, token, next, line) && isValid; /* Handle instruction on the first run */
            else
            {
                if (type == _TYPE_DATA)
                    return writeDataInstruction(next) && isValid; /* Write .data instruction */
                else if (type == _TYPE_STRING)
                    return writeStringInstruction(next) && isValid; /* Write .string instruction */
                else
                    return True;
            }
        }
    }
    else if (isOperation(token))
    {
        /* Handle assembly operations */
        char args[MAX_LINE_LEN] = {0};
        strcpy(args, (line + strlen(token)));                                                                   /* Copy the arguments after the operation */
        return (*globalState)() == firstRun ? handleOperation(token, args) : writeOperationBinary(token, args); /* Handle or write the operation */
    }
    else
    {
        /* Handle undefined tokens */
        if (strlen(token) > 1)
            return reportError(undefinedTokenNotOperationOrInstructionOrLabel); /* Error for unrecognized token */
        else
            return reportError(illegalApearenceOfCharacterInTheBegningOfTheLine); /* Error for illegal characters */
    }

    return isValid;
}

/**
 * handleSingleLine
 * ----------------
 * Handles a single line of assembly code by parsing it.
 * It increments the current line number after processing.
 *
 * Parameters:
 * - line: The line of code to be processed.
 *
 * Returns:
 * - Bool: True if the line is valid, False otherwise.
 */
Bool handleSingleLine(char *line)
{
    State (*globalState)() = &getGlobalState; /* Get the current state of the assembler */
    char lineCopy[MAX_LINE_LEN] = {0};        /* Copy of the current line for safe manipulation */
    Bool result = True;                       /* Assume the line is valid */
    char *token;
    strcpy(lineCopy, line);                                                                                  /* Create a copy of the line */
    token = ((*globalState)() == firstRun) ? strtok(lineCopy, " \t\n\f\r") : strtok(lineCopy, ", \t\n\f\r"); /* Extract the first token */
    result = parseLine(token, line);                                                                         /* Parse the line */
    (*currentLineNumberPlusPlus)();                                                                          /* Increment line number */
    return result;
}

/**
 * parseAssemblyCode
 * -----------------
 * Reads and processes each line of the source assembly file.
 * It handles multiple passes through the file, such as the first and second runs.
 *
 * Parameters:
 * - src: The source file to be parsed.
 */
void parseAssemblyCode(FILE *src)
{
    State (*globalState)() = &getGlobalState;  /* Get the current global state */
    void (*setState)(State) = &setGlobalState; /* Function pointer to set the global state */
    int c = 0, i = 0;                          /* Variables for reading characters and tracking position */
    char line[MAX_LINE_LEN] = {0};             /* Buffer for each line */
    Bool isValidCode = True;                   /* Assume the code is valid initially */
    State nextState;                           /* State to transition to after parsing */
    char *(*fileName)() = &getFileNamePath;    /* Get the file name */

    (*resetCurrentLineCounter)(); /* Reset the line counter at the beginning */

    if ((*globalState)() == secondRun)
        printf("\n\n\nSecond Run:(%s)\n", (*fileName)()); /* Output message for second run */
    else if ((*globalState)() == firstRun)
        printf("\n\n\nFirst Run:(%s)\n", (*fileName)()); /* Output message for first run */

    while (((c = fgetc(src)) != EOF))
    {
        if (isspace(c) && i > 0)
            line[i++] = ' '; /* Replace multiple spaces with a single space */

        else if (!isspace(c))
            line[i++] = c; /* Add characters to the line buffer */

        if (i >= MAX_LINE_LEN - 2)
            c = '\n'; /* Force newline if the line exceeds the maximum length */

        if (c == '\n')
        {
            if (i > 0)
            {
                isValidCode = handleSingleLine(line) && isValidCode; /* Parse the line */
                memset(line, 0, MAX_LINE_LEN);                       /* Clear the line buffer */
                i = 0;                                               /* Reset the index */
            }
        }
    }

    if (i > 0)
        isValidCode = handleSingleLine(line) && isValidCode; /* Parse the last line if necessary */

    if (!isValidCode)
        nextState = assemblyCodeFailedToCompile; /* Set the state to failure if the code is invalid */
    else
        nextState = (*globalState)() == firstRun ? secondRun : createOutputFiles; /* Transition to the next state */

    (*resetCurrentLineCounter)(); /* Reset the line counter */
    (*setState)(nextState);       /* Set the next state */
}
