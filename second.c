#include "data.h"

/**
 * writeOperationBinary
 * ----------
 * This function generates the binary representation of an operation in assembly code.
 * It takes the operation name and its arguments, determines the addressing methods
 * for each operand, and writes the appropriate machine code words to memory.
 *
 * Parameters:
 * - operationName: The name of the operation (e.g., "mov", "add").
 * - args: The arguments of the operation (e.g., registers, labels, or immediate values).
 *
 * Returns:
 * - Bool: True if the operation is valid and successfully written, False otherwise.
 */
Bool writeOperationBinary(char *operationName, char *args)
{
    const Operation *op = getOperationByName(operationName); /* Get the operation details by name */
    char *first, *second;
    AddrMethodsOptions active[2] = {{0, 0, 0, 0}, {0, 0, 0, 0}}; /* Array to hold addressing methods for each operand */
    Bool isValid = True;                                         /* Assume the operation is valid initially */

    first = strtok(args, ", \t\n\f\r");  /* Extract the first operand */
    second = strtok(NULL, ", \t\n\f\r"); /* Extract the second operand */

    if (first && second)
    {
        /* two operants are given */
        isValid = detectOperandType(first, active, 0) && detectOperandType(second, active, 1); /* Detect operand types */
        writeFirstWord(op, active);                                                            /* Write the first word of the instruction */
        writeSecondAndThirdWords(first, second, active, op);                                   /* Write the second and third words based on operands */
    }
    else if (!second && first)
    {
        /* Only one operand */
        second = first;
        isValid = detectOperandType(second, active, 1);     /* Detect the type of the single operand */
        writeFirstWord(op, active);                         /* Write the first word */
        writeSecondAndThirdWords(NULL, second, active, op); /* Write the second word */
    }
    else if (!first && !second)
    {
        /* No operands */
        writeFirstWord(op, active); /* Write only the first word (no operands) */
    }

    return isValid; /* Return whether the operation was valid */
}

/**
 * writeSecondAndThirdWords
 * ------------
 * Writes the second and third words of the machine code for an operation.
 * It handles operands that can be registers, indirect, direct, or immediate values.
 *
 * Parameters:
 * - first: The first operand (or NULL if there is none).
 * - second: The second operand.
 * - active: An array containing the addressing method for each operand.
 * - op: The operation being processed.
 */
void writeSecondAndThirdWords(char *first, char *second, AddrMethodsOptions active[2], const Operation *op)
{
    /* Handle cases where both operands are registers or indirect addressing, so only one operand word is needed */
    if ((active[0].reg && active[1].reg) || (active[0].reg && active[1].indirect) ||
        (active[0].indirect && active[1].reg) || (active[0].indirect && active[1].indirect))
    {
        addWord((parseRegNumberFromOperand(first) << 6) | (parseRegNumberFromOperand(second) << 3) | A, Code);
    }
    else
    {
        /* Process the first operand if exists */
        if (first)
        {
            if (active[0].reg || active[0].indirect)
                addWord((parseRegNumberFromOperand(first) << 6) | A, Code);
            else if (active[0].direct)
                writeDirectOperandWord(first);
            else if (active[0].immediate)
                writeImmediateOperandWord(first);
        }

        /* Process the second operand if exists */
        if (second)
        {
            if (active[1].reg || active[1].indirect)
                addWord(((parseRegNumberFromOperand(second) << 3) | A), Code);
            else if (active[1].direct)
                writeDirectOperandWord(second);
            else if (active[1].immediate)
                writeImmediateOperandWord(second);
        }
    }
}

/**
 * writeFirstWord
 * -------
 * Writes the first word of an operation, which contains the opcode and the addressing methods.
 *
 * Parameters:
 * - op: The operation to be written.
 * - active: An array containing the addressing method for each operand.
 */
void writeFirstWord(const Operation *op, AddrMethodsOptions active[2])
{
    unsigned srcAddrValue = 0, targetAddrValue = 0, firstWord = 0;

    /* Determine the addressing method for the source operand */
    if (active[0].immediate)
        srcAddrValue = IMMEDIATE_ADDR;
    else if (active[0].direct)
        srcAddrValue = DIRECT_ADDR;
    else if (active[0].indirect)
        srcAddrValue = INDIRECT_ADDR;
    else if (active[0].reg)
        srcAddrValue = REGISTER_DIRECT_ADDR;

    /* Determine the addressing method for the target operand */
    if (active[1].immediate)
        targetAddrValue = IMMEDIATE_ADDR;
    else if (active[1].direct)
        targetAddrValue = DIRECT_ADDR;
    else if (active[1].indirect)
        targetAddrValue = INDIRECT_ADDR;
    else if (active[1].reg)
        targetAddrValue = REGISTER_DIRECT_ADDR;

    /* Construct the first word with opcode and addressing methods */
    firstWord = A | (targetAddrValue << 3) | (srcAddrValue << 7) | (op->op << 11);
    addWord(firstWord, Code); /* Add the word to memory */
}

/**
 * writeDirectOperandWord
 * -----------
 * Writes the word corresponding to a direct addressing operand.
 *
 * Parameters:
 * - labelName: The name of the label or symbol for direct addressing.
 */
void writeDirectOperandWord(char *labelName)
{
    unsigned base = 0, address = 0;

    if (isExternal(labelName)) /* Check if the label is external */
    {
        base = getIC();
        addWord(E, Code);                                 /* Mark as external */
        updateExtPositionData(labelName, base, base + 1); /* Update external symbol data */
    }
    else
    {
        address = getSymbolAddress(labelName); /* Get the address of the symbol */
        addWord((address << 3) | R, Code);     /* Add the address word */
    }
}

/**
 * writeImmediateOperandWord
 * -------------
 * Writes the word corresponding to an immediate addressing operand.
 *
 * Parameters:
 * - n: The immediate value as a string.
 */
void writeImmediateOperandWord(char *n)
{
    n++;                                 /* Skip the '#' character */
    addWord(((atoi(n) << 3) | A), Code); /* Convert the immediate value to an integer and write it */
}

/**
 * detectOperandType
 * ---------
 * Detects the addressing method of an operand and sets the corresponding flags in the active array.
 *
 * Parameters:
 * - operand: The operand to detect.
 * - active: An array to store the addressing method flags.
 * - type: Indicates whether it's the source (0) or target (1) operand.
 *
 * Returns:
 * - Bool: True if the operand type is valid, False otherwise.
 */
Bool detectOperandType(char *operand, AddrMethodsOptions active[2], int type)
{
    if (!operand)
        return True;

    if (isValidImmediateParamter(operand))
        active[type].immediate = 1;
    else if (isRegistery(operand))
        active[type].reg = 1;
    else if (isValidIndirectParameter(operand))
        active[type].indirect = 1;
    else
    {
        if (isSymbolExist(operand))
        {
            if (isEntry(operand) && !isNonEmptyEntry(operand))
                return reportError(entryDeclaredButNotDefined);

            active[type].direct = 1;
        }
        else
            return reportError(labelNotExist);
    }
    return True;
}

/**
 * writeDataInstruction
 * ----------
 * Writes the binary representation of a .data instruction, converting the data values into machine code.
 *
 * Parameters:
 * - token: The first data value in the .data instruction.
 *
 * Returns:
 * - Bool: Always returns True.
 */
Bool writeDataInstruction(char *token)
{
    int num;
    while (token != NULL)
    {
        num = atoi(token);                  /* Convert the token to an integer */
        addWord(num, Data);                 /* Write the data to memory */
        token = strtok(NULL, ", \t\n\f\r"); /* Get the next data value */
    }
    return True;
}

/**
 * writeStringInstruction
 * -----------
 * Writes the binary representation of a .string instruction, converting the string characters into machine code.
 *
 * Parameters:
 * - s: The string to be written.
 *
 * Returns:
 * - Bool: Always returns True.
 */
Bool writeStringInstruction(char *s)
{
    char *start = strchr(s, '\"'); /* Find the opening quote of the string */
    int i, len;
    start++; /* Move past the opening quote */
    len = strlen(start);

    /* Write each character of the string */
    for (i = 0; i < len - 1; i++)
        addWord(start[i], Data);

    /* Write the null terminator for the string */
    addWord('\0', Data);
    return True;
}

/**
 * parseRegNumberFromOperand
 * -------------
 * Extracts the register number from an operand string (e.g., "r1" becomes 1).
 *
 * Parameters:
 * - s: The operand containing the register (e.g., "r1").
 *
 * Returns:
 * - int: The register number.
 */
int parseRegNumberFromOperand(char *s)
{
    s = strchr(s, 'r');           /* Find the 'r' character in the operand */
    return getRegisteryNumber(s); /* Get the register number */
}
