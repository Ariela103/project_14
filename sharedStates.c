#include "data.h"

/**
 * globalState: Keeps track of the current state of the program (e.g., startProgram, firstRun, etc.).
 * path: Stores the file path of the current source file being processed.
 * currentLineNumber: Tracks the current line number in the source file.
 */
static State state = startProgram;
static char *path;
static unsigned currentLineNumber = 1;

/**
 * setGlobalState
 * -------
 * Sets the global state of the program to the new state provided as an argument.
 *
 * Parameters:
 * - newState: The new state to set the program to.
 */
void setGlobalState(State newState)
{
    state = newState; /* Update the global state */
}

/**
 * getGlobalState
 * -------
 * Retrieves the current global state of the program.
 *
 * Returns:
 * - State: The current state of the program.
 */
State getGlobalState()
{
    State current = state;
    return current; /* Return the current global state */
}

/**
 * setFileNamePath
 * --------
 * Sets the file path to the source file being processed. If a path already exists,
 * the function reallocates memory to accommodate the new file path.
 *
 * Parameters:
 * - s: The new file path as a string.
 */
void setFileNamePath(char *s)
{
    if (!*s)
        return; /* If the provided string is empty, do nothing */

    /* Reallocate memory for the path and store the new file path */
    path = (char *)realloc(path, strlen(s) * sizeof(char *));
    strcpy(path, cloneString(s)); /* Clone the string and store it in 'path' */
}

/**
 * getFileNamePath
 * --------
 * Retrieves the current file path of the source file being processed.
 *
 * Returns:
 * - char*: A clone of the current file path.
 */
char *getFileNamePath()
{
    return cloneString(path); /* Return a cloned copy of the file path */
}

/**
 * resetCurrentLineNumber
 * -----------
 * Resets the current line number to 1. This is typically used at the start of
 * processing a new file or a new run of the assembler.
 */
void resetCurrentLineNumber()
{
    currentLineNumber = 1; /* Reset the line number counter to 1 */
}

/**
 * increaseCurrentLineNumber
 * -------------
 * Increments the current line number by 1. This is called after each line of code is processed.
 */
void increaseCurrentLineNumber()
{
    currentLineNumber++; /* Increment the line number counter */
}

/**
 * getCurrentLineNumber
 * ----------
 * Retrieves the current line number being processed in the source file.
 *
 * Returns:
 * - int: The current line number.
 */
int getCurrentLineNumber()
{
    return currentLineNumber; /* Return the current line number */
}
