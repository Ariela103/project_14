/*
-----------------------------------------------------------------------------------------
--------------------------- op.h General Overview: ---------------------------
-----------------------------------------------------------------------------------------

This header file defines the interface for working with assembly operations, including
their corresponding opcodes and allowed addressing modes. The operations supported by
the assembler are stored in a static array. Functions in this module allow checking for
valid operations, retrieving operation details by name or opcode, and validating operation
names.

-----------------------------------------------------------------------------------------
*/

/**
 * @brief Checks if the given operation name is valid.
 *
 * This function checks if the provided operation name exists in the operation table.
 *
 * @param s The operation name to validate (e.g., "mov").
 * @return Bool Returns True if the operation name is valid, False otherwise.
 */
Bool isOperationName(char *s);

/**
 * @brief Retrieves the operation details by name.
 *
 * This function returns a pointer to the `Operation` structure for the given operation name.
 *
 * @param s The operation name (e.g., "mov").
 * @return const Operation* A pointer to the `Operation` structure if found, or NULL if not.
 */
const Operation *getOperationByName(char *s);

/**
 * @brief Retrieves the operation details by index (opcode).
 *
 * This function returns a pointer to the `Operation` structure for the given opcode index.
 *
 * @param i The index (opcode) of the operation.
 * @return const Operation* A pointer to the `Operation` structure if valid, or NULL if out of range.
 */
const Operation *getOperationByIndex(unsigned int i);

/**
 * @brief Retrieves the operation's name based on a substring.
 *
 * This function returns the operation's name if the given string matches or is a substring
 * of an operation's keyword.
 *
 * @param s The string to search for (can be a substring).
 * @return const char* The operation's name if found, or NULL if no match is found.
 */
const char *getOperationName(char *s);

/**
 * @brief Retrieves the opcode index of the given operation name.
 *
 * This function returns the index (opcode) of the operation for the provided name.
 *
 * @param s The operation name to search for.
 * @return int The index (opcode) of the operation if found, or -1 if not.
 */
int getOpIndex(char *s);

/**
 * @brief Checks if the given operation name is a substring of any operation.
 *
 * This function checks if the provided string is a substring of any valid operation name.
 *
 * @param s The name or part of the name of the operation to check.
 * @return Bool Returns True if the operation is found as a substring, False otherwise.
 */
Bool isOperationNotStrict(char *s);