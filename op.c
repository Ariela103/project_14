#include "data.h"

/**
 * Array of supported operations with opcodes and addressing mode capabilities.
 * Each operation has:
 * - An opcode (e.g., 0 for "mov")
 * - A keyword (e.g., "mov")
 * - Allowed addressing methods for the source operand (direct, immediate, etc.)
 * - Allowed addressing methods for the destination operand
 */
static const Operation operations[OP_SIZE] = {
    {0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},  /* mov: allows various addressing modes */
    {1, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},  /* cmp: allows comparisons between operands */
    {2, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},  /* add: allows addition operations */
    {3, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},  /* sub: allows subtraction operations */
    {4, "lea", {0, 1, 0, 0}, {0, 1, 1, 1}},  /* lea: load effective address */
    {5, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},  /* clr: clears a register */
    {6, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},  /* not: performs bitwise negation */
    {7, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},  /* inc: increments a value */
    {8, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},  /* dec: decrements a value */
    {9, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},  /* jmp: jump to an address */
    {10, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}}, /* bne: branch if not equal */
    {11, "red", {0, 0, 0, 0}, {0, 1, 1, 1}}, /* red: reads an input */
    {12, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}}, /* prn: prints a value */
    {13, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}}, /* jsr: jump to subroutine */
    {14, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}}, /* rts: return from subroutine */
    {15, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}} /* stop: halts execution */
};

/**
 * isOperationName
 * ---------------
 * Checks if the given operation name is valid by checking if it exists in the operations table.
 *
 * Parameters:
 * - s: The name of the operation (e.g., "mov").
 *
 * Returns:
 * - Bool: True if the operation name is valid, False if it isn't.
 */
Bool isOperationName(char *s)
{
    return getOpIndex(s) != -1 ? True : False;
}

/**
 * getOperationByName
 * ------------------
 * Returns the information about the operation associated with the given name.
 *
 * Parameters:
 * - s: The name of the operation (e.g., "mov").
 *
 * Returns:
 * - const Operation*: A pointer to the Operation structure if the operation is valid.
 * - NULL: If the operation does not exist.
 */
const Operation *getOperationByName(char *s)
{
    int i = getOpIndex(s);

    /* Return the operation details if found, otherwise return NULL */
    if (i != -1)
        return &operations[i];

    return NULL;
}

/**
 * getOperationByIndex
 * -------------------
 * Retrieves the operation based on its index (opcode).
 *
 * Parameters:
 * - i: The opcode index of the operation.
 *
 * Returns:
 * - const Operation*: A pointer to the Operation structure if the index is valid.
 * - NULL: If the index is out of range.
 */
const Operation *getOperationByIndex(unsigned int i)
{
    /* Ensure the index is within the bounds of the operations array */
    return i < OP_SIZE ? &operations[i] : NULL;
}

/**
 * getOperationName
 * ----------------
 * Returns the operation's keyword (name) if the given string matches or is a substring of any operation.
 *
 * Parameters:
 * - s: The string to search for (can be a substring).
 *
 * Returns:
 * - const char*: The operation's name if found.
 * - 0: If no match is found.
 */
const char *getOperationName(char *s)
{
    int i = 0;
    /* Search through the operations array to find the matching operation name */
    while (i < OP_SIZE)
    {
        if (strstr(operations[i].keyword, s) != NULL)
            return operations[i].keyword;
        i++;
    }
    return 0;
}

/**
 * getOpIndex
 * ----------
 * Returns the opcode index of the given operation name.
 *
 * Parameters:
 * - s: The name of the operation to search for.
 *
 * Returns:
 * - int: The index (opcode) of the operation if found.
 * - -1: If the operation is not found.
 */
int getOpIndex(char *s)
{
    int i = 0;
    /* Iterate through the operations to find the matching operation name */
    while (i < OP_SIZE)
    {
        if (strcmp(operations[i].keyword, s) == 0)
            return i;
        i++;
    }
    return -1;
}

/**
 * isOperationNotStrict
 * --------------------
 * Checks if the given operation name is a substring of any defined operation.
 *
 * Parameters:
 * - s: The name or part of the name of the operation to check.
 *
 * Returns:
 * - Bool: True if the operation is found as a substring in any operation name.
 * - False: If no match is found.
 */
Bool isOperationNotStrict(char *s)
{
    int i = 0;
    /* Search for the substring in the operation keywords */
    while (i < OP_SIZE)
    {
        if (strstr(operations[i].keyword, s) != NULL)
            return True;
        i++;
    }
    return False;
}
