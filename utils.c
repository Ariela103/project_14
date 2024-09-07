#include "data.h"

/**
 * Array of register names used in the assembly language.
 * These are common register names r0 to r7.
 */
const char *regs[REGS_SIZE] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/**
 * isMacroOpening
 * --------------
 * Checks if the given string is the opening of a macro.
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is "macr" (macro opening), False otherwise.
 */
Bool isMacroOpening(char *s)
{
    return !strcmp(s, "macr") ? True : False;
}

/**
 * isMacroClosing
 * --------------
 * Checks if the given string is the closing of a macro.
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is "endmacr" (macro closing), False otherwise.
 */
Bool isMacroClosing(char *s)
{
    return !strcmp(s, "endmacr") ? True : False;
}

/**
 * isPossiblyUseOfMacro
 * --------------------
 * Checks if the given string could possibly be a macro usage.
 * Ensures the string is not a label, operation, comment, instruction, or macro declaration.
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string could be a macro usage, False otherwise.
 */
Bool isPossiblyUseOfMacro(char *s)
{
    return !isLabelDeclaration(s) && !isOperation(s) && !isComment(s) && !isInstructionStrict(s) && !isMacroClosing(s) && !isMacroOpening(s);
}

/**
 * isLegalMacroName
 * ----------------
 * Checks if the given string is a valid macro name.
 * A valid macro name is one that is not an instruction or operation.
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is a legal macro name, False otherwise.
 */
Bool isLegalMacroName(char *s)
{
    return !isInstructionStrict(s) && !isOperation(s) ? True : False;
}

/**
 * isInstruction
 * -------------
 * Checks if the given string is an instruction.
 * It handles both exact matches and partial matches when there are missing spaces.
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is an instruction (e.g., .data, .string), False otherwise.
 */
Bool isInstruction(char *s)
{
    if ((!strcmp(s, DATA) || !strcmp(s, STRING) || !strcmp(s, ENTRY) || !strcmp(s, EXTERNAL)))
        return True;

    else if (strstr(s, DATA) != NULL || strstr(s, STRING) != NULL || strstr(s, ENTRY) != NULL || strstr(s, EXTERNAL) != NULL)
    {
        /* Handles cases where there is a missing space after the instruction */
        return True;
    }
    else
        return False;
}

/**
 * isInstructionStrict
 * -------------------
 * Checks if the given string is strictly an instruction without any missing spaces.
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is exactly an instruction (e.g., .data, .string), False otherwise.
 */
Bool isInstructionStrict(char *s)
{
    return ((!strcmp(s, DATA) || !strcmp(s, STRING) || !strcmp(s, ENTRY) || !strcmp(s, EXTERNAL))) ? True : False;
}

/**
 * isRegistery
 * -----------
 * Checks if the given string is a valid register name (r0-r7).
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is a valid register, False otherwise.
 */
Bool isRegistery(char *s)
{
    int len = strlen(s);
    int i = 0;
    if (s[0] == 'r' && len == 2)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(regs[i], s) == 0))
                return True;
            i++;
        }
    }
    return False;
}

/**
 * isValidImmediateParamter
 * ------------------------
 * Checks if the given string is a valid immediate parameter (e.g., #5, #-10).
 * Valid immediate parameters start with a '#' followed by an optional sign and digits.
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is a valid immediate parameter, False otherwise.
 */
Bool isValidImmediateParamter(char *s)
{
    int i, len = strlen(s);
    if (len < 2 || s[0] != '#' || (!(s[1] == '-' || s[1] == '+' || isdigit(s[1]))))
        return False;
    for (i = 2; i < len; i++)
        if (!isdigit(s[i]))
            return False;
    return True;
}

/**
 * isIndirectParameter
 * -------------------
 * Checks if the given string is a valid indirect parameter (e.g., *r0).
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is a valid indirect parameter, False otherwise.
 */
Bool isIndirectParameter(char *s)
{
    int len = strlen(s);
    return (len == 3 && s[0] == '*' && s[1] == 'r');
}

/**
 * isValidIndirectParameter
 * ------------------------
 * Validates if the string is a valid indirect parameter and if it references a valid register (r0-r7).
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is a valid indirect parameter, False otherwise.
 */
Bool isValidIndirectParameter(char *s)
{
    if (isIndirectParameter(s) == True)
    {
        if (s[2] >= '0' && s[2] <= '7')
            return True;
    }
    return False;
}

/**
 * isComment
 * ---------
 * Checks if the given string is a comment (starts with ';').
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is a comment, False otherwise.
 */
Bool isComment(char *s)
{
    s = trimFromLeft(s);
    return s[0] == ';' ? True : False;
}

/**
 * isOperation
 * -----------
 * Checks if the given string is a valid operation name.
 * Looks up the operation by its name.
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is a valid operation, False otherwise.
 */
Bool isOperation(char *s)
{
    return (getOperationByName(s) != NULL) ? True : False;
}

/**
 * isLabelDeclarationStrict
 * ------------------------
 * Checks if the given string is a strict label declaration (ends with ':').
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is a label declaration, False otherwise.
 */
Bool isLabelDeclarationStrict(char *s)
{
    return s[strlen(s) - 1] == ':' ? True : False;
}

/**
 * isLabelDeclaration
 * ------------------
 * Checks if the given string contains a label declaration (contains ':').
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - Bool: True if the string is a label declaration, False otherwise.
 */
Bool isLabelDeclaration(char *s)
{
    return strchr(s, ':') != NULL ? True : False;
}

/**
 * getInstructionType
 * ------------------
 * Determines the instruction type based on the given string (e.g., .data, .string).
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - int: The type of instruction (_TYPE_DATA, _TYPE_STRING, etc.) or False if not an instruction.
 */
int getInstructionType(char *s)
{
    if (strstr(s, DATA) != NULL)
        return _TYPE_DATA;
    if (strstr(s, STRING) != NULL)
        return _TYPE_STRING;
    if (strstr(s, ENTRY) != NULL)
        return _TYPE_ENTRY;
    if (strstr(s, EXTERNAL) != NULL)
        return _TYPE_EXTERNAL;
    return False;
}

/**
 * getRegisteryNumber
 * ------------------
 * Extracts the register number from the given register string (e.g., r2 -> 2).
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - int: The register number.
 */
int getRegisteryNumber(char *s)
{
    s++;
    return atoi(s);
}

/**
 * getInstructionNameByType
 * ------------------------
 * Returns the instruction name corresponding to the given type.
 *
 * Parameters:
 * - type: The instruction type (_TYPE_DATA, _TYPE_STRING, etc.).
 *
 * Returns:
 * - char*: The instruction name corresponding to the type.
 */
char *getInstructionNameByType(int type)
{
    switch (type)
    {
    case _TYPE_DATA:
        return strcat(DATA, "\0");

    case _TYPE_STRING:
        return strcat(STRING, "\0");

    case _TYPE_ENTRY:
        return strcat(ENTRY, "\0");

    case _TYPE_EXTERNAL:
        return strcat(EXTERNAL, "\0");

    default:
        break;
    }

    return NULL;
}

/**
 * getInstructionName
 * ------------------
 * Returns the instruction name based on the given string.
 *
 * Parameters:
 * - s: The string to check.
 *
 * Returns:
 * - char*: The instruction name or 0 if not found.
 */
char *getInstructionName(char *s)
{
    if (strstr(s, DATA) != NULL)
        return DATA;
    if (strstr(s, STRING) != NULL)
        return STRING;
    if (strstr(s, ENTRY) != NULL)
        return ENTRY;
    if (strstr(s, EXTERNAL) != NULL)
        return EXTERNAL;

    return 0;
}

/**
 * verifyLabelNaming
 * -----------------
 * Verifies that the label name is valid (starts with a letter, does not exceed the max length, and contains only alphanumeric characters).
 *
 * Parameters:
 * - s: The label name to verify.
 *
 * Returns:
 * - Bool: True if the label name is valid, False otherwise.
 */
Bool verifyLabelNaming(char *s)
{
    int i = 0;
    int labelLength = strlen(s);

    /* Label must start with an alphabet character */
    if (isalpha(s[0]) == 0)
        return False;

    /* Label length must not exceed the maximum length */
    if (labelLength > MAX_LABEL_LEN || labelLength < 1)
        return False;

    /* Label must not be a reserved register or operation name */
    if (isRegistery(s))
        return False;

    else if (isOperationName(s))
        return False;

    /* Label must only contain alphanumeric characters */
    while (i < labelLength)
    {
        if (!isalnum(s[i]))
            return False;

        i++;
    }

    return True;
}

/**
 * verifyLabelNamingAndPrintErrors
 * -------------------------------
 * Verifies that the label name is valid and prints errors if it is not.
 *
 * Parameters:
 * - s: The label name to verify.
 *
 * Returns:
 * - Bool: True if the label name is valid, False otherwise (with error messages).
 */
Bool verifyLabelNamingAndPrintErrors(char *s)
{
    int i = 0;
    int labelLength = strlen(s);

    /* Label must start with an alphabet character */
    if (isalpha(s[0]) == 0)
        return reportError(illegalLabelNameUseOfCharacters);

    /* Label length must not exceed the maximum length */
    else if (labelLength > MAX_LABEL_LEN)
        return reportError(illegalLabelNameLength);
    else if (labelLength < 1)
        return reportError(illegalLabelNameLength);

    /* Label must not be a reserved register or operation name */
    else if (isRegistery(s))
        return reportError(illegalLabelNameUseOfSavedKeywordUsingRegisteryName);

    else if (isOperationName(s))
        return reportError(illegalLabelNameUseOfSavedKeywordUsingOperationName);

    /* Label must only contain alphanumeric characters */
    while (i < labelLength)
    {
        if (!isalnum(s[i]))
            return reportError(illegalLabelNameUseOfCharacters);

        i++;
    }

    return True;
}
