/*
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
--------------------------- sharedState.h General Overview: ----------------------------
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------

The `sharedState` module is responsible for safely managing and sharing the global state of the assembler
at any given time. It provides functionality to track:

1. **The current state of the program**: This includes whether the program is in a particular phase, such
   as macro parsing, the first pass, the second pass, or handling errors.

2. **The current source file path**: This is the name and location of the file currently being processed by
   the assembler.

3. **The current line number**: This keeps track of the line number within the source file, which is useful
   for error reporting and managing the assembler's progress through the file.

By using getter and setter functions, the state management is encapsulated within the module, and the rest
of the program interacts with these global variables exclusively through these functions.

-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

/**
 * setGlobalState
 * --------------
 * Updates the global state of the program to the provided state.
 *
 * Parameters:
 * - newState: The new state to set the program to.
 */
void setGlobalState(State newState);

/**
 * getGlobalState
 * --------------
 * Retrieves the current global state of the program.
 *
 * Returns:
 * - State: The current global state.
 */
State getGlobalState();

/**
 * setFileNamePath
 * ---------------
 * Updates the file path of the source file currently being processed.
 *
 * Parameters:
 * - s: The file path of the current source file as a string.
 */
void setFileNamePath(char *s);

/**
 * getFileNamePath
 * ---------------
 * Retrieves the current file path of the source file being processed.
 *
 * Returns:
 * - char*: A cloned copy of the current file path.
 */
char *getFileNamePath();

/**
 * resetCurrentLineNumber
 * ----------------------
 * Resets the current line number to 1. Typically used at the start of processing a new file.
 */
void resetCurrentLineNumber();

/**
 * increaseCurrentLineNumber
 * -------------------------
 * Increments the current line number by 1 after each processed line of code.
 */
void increaseCurrentLineNumber();

/**
 * getCurrentLineNumber
 * --------------------
 * Retrieves the current line number being processed in the source file.
 *
 * Returns:
 * - int: The current line number.
 */
int getCurrentLineNumber();