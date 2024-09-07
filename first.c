#include "data.h"

/**
 * handleOperation
 * ----------------
 * This function processes an operation based on its name and the operands provided as arguments.
 * It verifies the syntax of the operands, parses them, and checks whether they are legal for the given operation.
 *
 * Parameters:
 * - operationName: The name of the operation to be handled.
 * - args: The arguments/operands passed to the operation.
 *
 * Returns:
 * - Bool: Returns True if the operation is valid and processed successfully, False otherwise.
 */
Bool handleOperation(char *operationName, char *args)
{
    const Operation *p = getOperationByName(operationName);      /* Retrieve the operation details by name */
    AddrMethodsOptions active[2] = {{0, 0, 0, 0}, {0, 0, 0, 0}}; /* Initialize the addressing methods for two operands */
    char *first = 0;                                             /* First operand */
    char *second = 0;                                            /* Second operand */
    char *extra = 0;                                             /* Extra operand if any (error condition) */
    Bool areOperandsLegal;

    /* If arguments exist, check if the comma syntax is valid */
    if (*args)
        areOperandsLegal = verifyCommaSyntax(args);

    /* Extract the first operand using strtok */
    first = strtok(args, ", \t\n\f\r");
    if (first)
    {
        /* Extract the second operand, if available */
        second = strtok(NULL, ", \t\n\f\r");
        if (second)
        {
            /* If there is any extra operand, it's considered an error */
            extra = strtok(NULL, ", \t\n\f\r");
            if (extra)
                areOperandsLegal = reportError(extraOperandsPassed);
        }
        else
            second = 0; /* No second operand */
    }

    /* Parse the operands and check if they are valid for the operation */
    areOperandsLegal = parseOperands(first, second, p, active) && areOperandsLegal;

    /* If the operands are legal, calculate the size of the operation in memory */
    if (areOperandsLegal)
    {
        int size = 1; /* The size starts at 1 word */

        /* Check if any addressing method is used for the operands */
        if ((!active[0].direct && !active[0].immediate && !active[0].indirect && !active[0].reg) && (!active[1].direct && !active[1].immediate && !active[1].indirect && !active[1].reg))
            size = 1; /* No operands require addressing, size remains 1 */
        else if ((active[0].reg || active[0].indirect) && (active[1].reg || active[1].indirect))
            size = 2; /* Both operands are registers or indirect addresses, size is 2 */
        else
        {
            /* If there are two operands, size is 3, otherwise it's 2 */
            if (first && second)
                size = 3;
            else
                size = 2;
        }

        /* Reset the active addressing methods after processing */
        active[0].direct = active[0].immediate = active[0].indirect = active[0].reg = 0;
        active[1].direct = active[1].immediate = active[1].indirect = active[1].reg = 0;

        /* Increase the instruction counter by the calculated size */
        increaseInstructionCounter(size);
    }

    return areOperandsLegal;
}

/**
 * parseOperands
 * -------------
 * This function parses the source and destination operands for an operation,
 * ensuring they match the expected addressing modes defined in the operation structure.
 * It checks whether the correct number of operands is provided and validates each operand.
 *
 * Parameters:
 * - src: Source operand.
 * - des: Destination operand.
 * - op: The operation structure containing the allowed addressing modes.
 * - active: An array that tracks the addressing methods used by each operand.
 *
 * Returns:
 * - Bool: True if the operands match the expected types and are valid, False otherwise.
 */
Bool parseOperands(char *src, char *des, const Operation *op, AddrMethodsOptions active[2])
{
    int expectedOperandsCount = 0; /* Number of expected operands for this operation */
    int operandsPassedCount = 0;   /* Number of operands passed by the user */
    Bool isValid = True;           /* Tracks whether the operands are valid */

    /* Count how many operands were passed */
    if (src)
        operandsPassedCount++;
    if (des)
        operandsPassedCount++;

    /* Count how many operands the operation expects */
    if (op->src.direct || op->src.immediate || op->src.indirect || op->src.reg)
        expectedOperandsCount++;
    if (op->des.direct || op->des.immediate || op->des.indirect || op->des.reg)
        expectedOperandsCount++;

    /* If one operand is expected and passed, treat it as a destination */
    if (expectedOperandsCount == 1 && operandsPassedCount == 1)
    {
        des = src;
        src = 0;
    }

    /* If no operands are expected or passed, return True (valid) */
    if ((expectedOperandsCount == operandsPassedCount) && expectedOperandsCount == 0)
        return True;

    /* If too many operands are passed, log an error */
    if (operandsPassedCount > expectedOperandsCount)
        isValid = reportError(extraOperandsPassed);

    /* Validate source and destination operands against the allowed addressing methods */
    if ((op->src.direct || op->src.immediate || op->src.reg || op->src.indirect) && (op->des.direct || op->des.immediate || op->des.reg || op->des.indirect))
    {
        /* If a source operand is missing, log an error */
        if (!src)
            isValid = reportError(requiredSourceOperandIsMissin);
        else
            isValid = validateOperandMatch(op->src, active, src, 0) && isValid;

        /* If a destination operand is missing, log an error */
        if (!des)
            isValid = reportError(requiredDestinationOperandIsMissin);
        else
            isValid = validateOperandMatch(op->des, active, des, 1) && isValid;
    }
    else if (op->src.direct || op->src.immediate || op->src.reg || op->src.indirect)
    {
        /* Validate the source operand */
        if (!src)
            return reportError(requiredSourceOperandIsMissin);
        else
            return validateOperandMatch(op->src, active, src, 0) && isValid;
    }
    else if (op->des.direct || op->des.immediate || op->des.reg || op->des.indirect)
    {
        /* Validate the destination operand */
        if (!des)
            return reportError(requiredDestinationOperandIsMissin);
        else
            return validateOperandMatch(op->des, active, des, 1) && isValid;
    }

    return isValid;
}

/**
 * validateOperandMatch
 * --------------------
 * This function checks if the given operand matches one of the allowed addressing methods
 * for the operation and sets the appropriate flags in the `active` array.
 *
 * Parameters:
 * - allowedAddrs: Allowed addressing methods for the operation.
 * - active: Array that stores the active addressing method for the current operand.
 * - operand: The operand string.
 * - type: 0 for source operand, 1 for destination operand.
 *
 * Returns:
 * - Bool: True if the operand is valid, False otherwise.
 */
Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, AddrMethodsOptions active[2], char *operand, int type)
{
    /* Check which addressing mode the operand uses */
    Bool isAny = isValidImmediateParamter(operand) || isValidIndirectParameter(operand) || isRegistery(operand) || verifyLabelNaming(operand);

    Bool isImmediate = isValidImmediateParamter(operand);
    Bool isIndirect = !isImmediate && isValidIndirectParameter(operand);
    Bool isReg = !isIndirect && !isImmediate && isRegistery(operand);
    Bool isDirect = !isReg && !isIndirect && !isImmediate && verifyLabelNaming(operand);

    /* If no valid addressing mode is found, return an error */
    if (!isAny)
        return type == 1 ? reportError(illegalInputPassedAsOperandDesOperand) : reportError(illegalInputPassedAsOperandSrcOperand);

    /* Check if the operand uses an addressing mode that isn't allowed for this operation */
    else if (!allowedAddrs.reg && isReg)
        return type == 1 ? reportError(desOperandTypeIsNotAllowed) : reportError(srcOperandTypeIsNotAllowed);
    else if (!allowedAddrs.immediate && isImmediate)
        return type == 1 ? reportError(desOperandTypeIsNotAllowed) : reportError(srcOperandTypeIsNotAllowed);
    else if (!allowedAddrs.direct && isDirect)
        return type == 1 ? reportError(desOperandTypeIsNotAllowed) : reportError(srcOperandTypeIsNotAllowed);
    else if (!allowedAddrs.indirect && isIndirect)
        return type == 1 ? reportError(desOperandTypeIsNotAllowed) : reportError(srcOperandTypeIsNotAllowed);

    /* Set the active addressing method for the current operand */
    active[type].direct = isDirect;
    active[type].reg = isReg;
    active[type].immediate = isImmediate;
    active[type].indirect = isIndirect;

    return True;
}

/**
 * handleInstruction
 * -----------------
 * This function processes an assembly instruction or directive based on the type provided.
 * It checks whether the instruction is of type data, string, entry, or external, and handles each case accordingly.
 *
 * Parameters:
 * - type: The type of instruction (data, string, entry, or external).
 * - firstToken: The first token of the line (usually the instruction).
 * - nextTokens: The rest of the tokens following the first (typically arguments).
 * - line: The full line being processed.
 *
 * Returns:
 * - Bool: True if the instruction is handled successfully, False otherwise.
 */
Bool handleInstruction(int type, char *firstToken, char *nextTokens, char *line)
{
    /* Check if the first token is a valid instruction */
    if (isInstruction(firstToken))
    {
        /* If the instruction is of type data, verify and count data arguments */
        if (type == _TYPE_DATA)
        {
            return countAndVerifyDataArguments(line) ? True : False;
        }
        /* If the instruction is of type string, verify and count string arguments */
        else if (type == _TYPE_STRING)
            return countAndVerifyStringArguments(line) ? True : False;

        /* Handle entry and external instructions */
        if (type == _TYPE_ENTRY || type == _TYPE_EXTERNAL)
        {
            /* Check if the next token (label) exists */
            if (nextTokens)
            {
                char *labelName = cloneString(nextTokens); /* Clone the label name */
                nextTokens = strtok(NULL, " \t\n\f\r");    /* Check if more tokens exist */

                /* If there are extra tokens, log an error */
                if (nextTokens)
                {
                    reportError(illegalApearenceOfCharactersInTheEndOfTheLine);
                    return False;
                }
                else
                {
                    /* Handle entry or external label definition */
                    if (type == _TYPE_ENTRY)
                        return addSymbol(labelName, 0, 0, 0, 1, 0) ? True : False;
                    if (type == _TYPE_EXTERNAL)
                        return addSymbol(labelName, 0, 0, 0, 0, 1) ? True : False;
                }

                free(labelName); /* Free the cloned label name after processing */
            }
            else
            {
                /* Log an error if no label is provided for entry/external declaration */
                reportError(emptyDeclaretionOfEntryOrExternalVariables);
                return False;
            }
        }
    }
    /* Handle label declarations */
    else if (isLabelDeclaration(firstToken))
    {
        int dataCounter = getDC(); /* Get the data counter for the current address */
        Bool isLabelNameAvailable;

        /* Remove the ':' at the end of the label declaration */
        firstToken[strlen(firstToken) - 1] = '\0';

        /* Check if the label name is already taken */
        isLabelNameAvailable = !isLabelNameAlreadyTaken(firstToken, Symbol);
        if (!isLabelNameAvailable)
            reportError(illegalSymbolNameAlreadyInUse);

        /* Add the label to the symbol table if the type is data or string */
        if (((type == _TYPE_DATA && countAndVerifyDataArguments(line)) || (type == _TYPE_STRING && countAndVerifyStringArguments(line))) && isLabelNameAvailable)
        {
            return addSymbol(firstToken, dataCounter, 0, 1, 0, 0) ? True : False;
        }
        else
            return False;
    }
    else
    {
        /* Log an error if the operation or instruction is undefined */
        reportError(undefinedOperation);
    }

    return False;
}

/**
 * handleLabel
 * -----------
 * This function processes a label and its corresponding instruction or operation.
 * It checks whether the next token after the label is an instruction, operation, or something else.
 *
 * Parameters:
 * - labelName: The name of the label being processed.
 * - nextToken: The next token after the label (typically the instruction or operation).
 * - line: The full line being processed.
 *
 * Returns:
 * - Bool: True if the label and instruction/operation are processed successfully, False otherwise.
 */
Bool handleLabel(char *labelName, char *nextToken, char *line)
{
    Bool isValid = True; /* Initialize the validity of the label and instruction */

    /* Check if the necessary parameters are present */
    if (!labelName || !nextToken || !line)
        return False;

    /* If the next token is an instruction, handle it based on its type */
    if (isInstruction(nextToken))
    {
        int instruction = getInstructionType(nextToken); /* Get the instruction type */

        /* Check if there is a missing space between the instruction and arguments */
        if (!isInstructionStrict(nextToken))
        {
            isValid = reportError(missinSpaceAfterInstruction); /* Log a warning if there's no space */
            nextToken = getInstructionNameByType(instruction);  /* Correct the instruction name */
        }

        /* Handle entry and external declarations */
        if (instruction == _TYPE_ENTRY || instruction == _TYPE_EXTERNAL)
        {
            char *next = strtok(NULL, " \t\n\f\r"); /* Get the next token (label or extra arguments) */

            if (next)
                return handleInstruction(instruction, nextToken, next, line) && isValid;
            else
                return reportWarning(emptyLabelDecleration); /* Log a warning if the label declaration is empty */
        }
        else
            return handleInstruction(instruction, labelName, nextToken, line) && isValid; /* Handle normal instructions */
    }

    /* If the next token is an operation, handle it accordingly */
    else if (isOperation(nextToken))
    {
        int icAddr = getIC();                                          /* Get the current instruction counter */
        char args[MAX_LINE_LEN] = {0};                                 /* Initialize argument buffer */
        int offset = (int)(strlen(labelName) + strlen(nextToken) + 1); /* Calculate offset in the line */

        /* Copy the arguments from the line, starting after the label and operation */
        strcpy(args, &line[offset]);

        /* If the operation is handled successfully, add the label to the symbol table */
        if (handleOperation(nextToken, args))
            return addSymbol(labelName, icAddr, 1, 0, 0, 0) ? True : False;
        else
            return False;
    }
    else
    {
        /* Log an error if the label is not followed by an expected operation or instruction */
        reportError(illegalLabelUseExpectedOperationOrInstruction);
    }

    return False;
}