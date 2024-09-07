#include "data.h"

/**
 * Function pointers to get and set the global state of the assembly process.
 * setState: Used to update the current state.
 * globalState: Used to retrieve the current state.
 */
void (*setState)(State) = &setGlobalState;
State (*globalState)() = &getGlobalState;

/** Function declarations for macro processing **/
extern Bool isPossiblyUseOfMacro(char *s);
extern Bool isMacroOpening(char *s);
extern Bool isMacroClosing(char *s);
extern Bool isLegalMacroName(char *s);
extern Item *addMacro(char *name, int start, int end);
extern Item *updateMacro(char *name, int start, int end);
extern Item *getMacro(char *s);

/**
 * parseMacros
 * -----------
 * This function handles macro parsing in the assembly source file. It checks for macro
 * declarations, verifies macro names, and processes macro invocations. If a macro is
 * detected, it is either defined or its content is substituted during the assembly process.
 *
 * Parameters:
 * - line: The current line being processed.
 * - token: The first token in the line (could be a macro or regular instruction).
 * - src: The source file being parsed.
 * - target: The target file where parsed macros or instructions will be written.
 */
void parseMacros(char *line, char *token, FILE *src, FILE *target)
{
    /**
     * Function pointer to increment the line number after each successful parsing.
     */
    void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;

    /**
     * Static variables to track macro name, and whether we are currently reading a macro.
     * 'start' and 'end' store the positions in the file where the macro definition begins and ends.
     */
    static char macroName[MAX_LABEL_LEN] = {0}, *next;
    static Bool isReadingMacro = False;
    static long start = 0, end = 0;

    /**
     * If we are not currently reading a macro and the token is not a macro opening,
     * write the line directly to the target file and move to the next line.
     */
    if (!isReadingMacro)
    {
        if (!isMacroOpening(token))
        {
            fprintf(target, "%s", line);
            (*currentLineNumberPlusPlus)();
        }
    }

    /**
     * If the token is neither a macro use, macro opening, nor macro closing, return immediately.
     */
    if (!isPossiblyUseOfMacro(token) && !isMacroOpening(token) && !isMacroClosing(token))
        return;

    /**
     * If a macro opening is detected, begin reading the macro.
     * Validate the macro name, and start tracking the macro definition from the current position.
     */
    if (isMacroOpening(token))
    {
        next = strtok(NULL, " \t\n\f\r"); /* Get the macro name */

        if (next == NULL) /* Error: macro declaration without a name */
        {
            reportError(macroDeclaretionWithoutDefiningMacroName);
            (*setState)(assemblyCodeFailedToCompile);
            return;
        }
        if (!isLegalMacroName(next)) /* Error: illegal macro name (e.g., reserved keyword) */
        {
            reportError(illegalMacroNameUseOfSavedKeywords);
            (*setState)(assemblyCodeFailedToCompile);
            return;
        }

        /**
         * Store the start position of the macro in the source file and copy the macro name.
         */
        start = ftell(src);
        strcpy(macroName, next);
        isReadingMacro = True;
    }
    /**
     * If a macro closing is detected, mark the end position of the macro,
     * add the macro to the table, and reset variables.
     */
    else if (isMacroClosing(token))
    {
        end = ftell(src) - strlen(line) - 2; /* Adjust the end position to exclude the closing line */
        addMacro(macroName, start, end);     /* Add the macro to the macro table */
        isReadingMacro = False;
        start = end = 0;                     /* Reset the start and end positions */
        memset(macroName, 0, MAX_LABEL_LEN); /* Clear the macro name */
    }
    /**
     * If a macro invocation is detected, replace the macro usage with its definition.
     */
    else if (isPossiblyUseOfMacro(token))
    {
        Item *p = getMacro(token); /* Retrieve the macro definition */
        if (p != NULL)
        {
            long c, toCopy = p->val.m.end - p->val.m.start; /* Calculate the length of the macro */
            long lastPosition = 0;

            /**
             * Move the file cursor to overwrite the current line with the macro content.
             */
            fseek(target, -strlen(line) - 1, SEEK_CUR);
            fprintf(target, "%s", "\0"); /* Overwrite the line */
            lastPosition = ftell(src);   /* Save the current position in the source file */

            /**
             * Copy the macro definition from the source file to the target file.
             */
            fseek(src, p->val.m.start, SEEK_SET);
            while (--toCopy && (c = fgetc(src)) != EOF)
                fputc(c, target);

            /**
             * Move back to the original position in the source file to continue processing.
             */
            fseek(src, lastPosition, SEEK_SET);
        }
    }
}

/**
 * parseSourceFile
 * ---------------
 * This function reads through the entire source file line by line, parsing each line
 * for macros and other instructions. It writes the processed output to the target file.
 *
 * Parameters:
 * - src: The source file being parsed.
 * - target: The target file where the processed output will be written.
 */
void parseSourceFile(FILE *src, FILE *target)
{
    char line[MAX_LINE_LEN] = {0};      /* Buffer to hold each line from the source file */
    char lineClone[MAX_LINE_LEN] = {0}; /* A clone of the line for safe tokenization */
    char *token, c;
    int i = 0;

    /**
     * Function pointer to reset the line counter before starting the parsing process.
     */
    void (*resetCurrentLineCounter)() = &resetCurrentLineNumber;
    (*resetCurrentLineCounter)(); /* Reset the line counter */

    /**
     * Read the source file character by character until the end of the file.
     */
    while (((c = fgetc(src)) != EOF))
    {
        line[i++] = c; /* Add the character to the current line buffer */

        /**
         * If the line exceeds the maximum length, forcefully add a newline.
         */
        if (i >= MAX_LINE_LEN - 2 && !isspace(c))
            c = '\n';

        /**
         * If the global state indicates a failure, stop processing.
         */
        if ((*globalState)() == assemblyCodeFailedToCompile)
            return;

        /**
         * Once a newline character is encountered, process the line.
         */
        if (c == '\n')
        {
            if (i > 0)
            {
                strncpy(lineClone, line, i);            /* Clone the line for tokenization */
                token = strtok(lineClone, " \t\n\f\r"); /* Get the first token */
                if (token != NULL)
                    parseMacros(line, token, src, target); /* Parse the line for macros */

                /**
                 * Clear the buffers and reset the index for the next line.
                 */
                memset(lineClone, 0, i);
                memset(line, 0, i);
                i = 0;
            }
        }
    }

    /**
     * If there are any remaining characters in the line buffer, process the last line.
     */
    if (i > 0)
    {
        strcpy(lineClone, line);
        token = strtok(lineClone, " \t\n\f\r");
        if (token != NULL)
            parseMacros(line, token, src, target);
    }

    /**
     * If no failure occurred, set the global state to indicate the first run is complete.
     */
    if ((*globalState)() != assemblyCodeFailedToCompile)
        (*setState)(firstRun);
}
