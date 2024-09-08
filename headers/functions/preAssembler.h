/*
-----------------------------------------------------------------------------------------
--------------------------- preAssembler.c/h General Overview: ---------------------------
-----------------------------------------------------------------------------------------
The `preAssembler.c` is responsible for handling macros within the source assembly file before
the main assembly process begins. It parses through the input source file, detecting and
processing macro definitions and macro invocations.

Key Functions:
- **parseSourceFile:** Reads the source file line by line and calls `parseMacros` to handle
  macro definitions and invocations.
- **parseMacros:** Processes macros, either by storing macro definitions or replacing
  macro invocations with the corresponding content.

By the end of the pre-assembly phase, the source file is processed and expanded with macro
definitions replaced, ensuring that the rest of the assembly process can proceed without needing
to handle macros.

-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

/* Function pointer declarations for handling global state */
extern void (*setState)(State);
extern State (*globalState)();

/**
 * parseMacros
 * -----------
 * Processes macro declarations and invocations in the source file.
 * This function checks for macros, stores macro definitions, and replaces macro
 * invocations with their corresponding content.
 *
 * Parameters:
 * - line: The current line from the source file being processed.
 * - token: The first token in the line, which could be a macro name or instruction.
 * - src: The source file being parsed.
 * - target: The output file where processed lines are written.
 */
void parseMacros(char *line, char *token, FILE *src, FILE *target);

/**
 * parseSourceFile
 * ---------------
 * Reads through the entire source file and processes each line, checking for macros.
 * It writes the processed content, with macros expanded, into the target file.
 *
 * Parameters:
 * - src: The source file being parsed.
 * - target: The target file where the processed content will be written.
 */
void parseSourceFile(FILE *src, FILE *target);
