#include "data.h"

Bool writeOperationBinary(char *operationName, char *args)
{
    const Operation *op = getOperationByName(operationName);
    char *first, *second;
    AddrMethodsOptions active[2] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
    Bool isValid = True;
    first = strtok(args, ", \t\n\f\r");
    second = strtok(NULL, ", \t\n\f\r");

    if (first && second)
    {
        isValid = detectOperandType(first, active, 0) && detectOperandType(second, active, 1);
        writeFirstWord(op, active);
        writeSecondAndThirdWords(first, second, active, op);
    }

    else if (!second && first)
    {
        second = first;
        isValid = detectOperandType(second, active, 1);
        writeFirstWord(op, active);
        writeSecondAndThirdWords(NULL, second, active, op);
    }
    else if (!first && !second)
        writeFirstWord(op, active);

    return isValid;
}

void writeSecondAndThirdWords(char *first, char *second, AddrMethodsOptions active[2], const Operation *op)
{
    /* second word contains two registeries operands*/
    if ((active[0].reg && active[1].reg) || (active[0].reg && active[1].indirect) || (active[0].indirect && active[1].reg) || (active[0].indirect && active[1].indirect))
        addWord((parseRegNumberFromOperand(first) << 6) | (parseRegNumberFromOperand(second) << 3) | A, Code);
    else
    {
        if (first)
        {
            if (active[0].reg || active[0].indirect)
                addWord((parseRegNumberFromOperand(first) << 6) | A, Code);
            else if (active[0].direct)
                writeDirectOperandWord(first);
            else if (active[0].immediate)
                writeImmediateOperandWord(first);
        }

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

void writeFirstWord(const Operation *op, AddrMethodsOptions active[2])
{
    unsigned srcAddrValue = 0, targetAddrValue = 0, firstWord = 0;

    if (active[0].immediate)
        srcAddrValue = IMMEDIATE_ADDR;
    else if (active[0].direct)
        srcAddrValue = DIRECT_ADDR;
    else if (active[0].indirect)
        srcAddrValue = INDIRECT_ADDR;
    else if (active[0].reg)
        srcAddrValue = REGISTER_DIRECT_ADDR;

    if (active[1].immediate)
        targetAddrValue = IMMEDIATE_ADDR;
    else if (active[1].direct)
        targetAddrValue = DIRECT_ADDR;
    else if (active[1].indirect)
        targetAddrValue = INDIRECT_ADDR;
    else if (active[1].reg)
        targetAddrValue = REGISTER_DIRECT_ADDR;

    firstWord = A | (targetAddrValue << 3) | (srcAddrValue << 7) | (op->op << 11);
    addWord(firstWord, Code);
}

void writeDirectOperandWord(char *labelName)
{

    unsigned base = 0, address = 0;
    if (isExternal(labelName))
    {
        base = getIC();
        addWord(E, Code);
        updateExtPositionData(labelName, base, base + 1);
    }
    else
    {
        address = getSymbolAddress(labelName);
        addWord((address << 3) | R, Code);
    }
}

void writeImmediateOperandWord(char *n)
{
    n++;
    addWord(((atoi(n) << 3) | A), Code);
}

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
                return yieldError(entryDeclaredButNotDefined);

            active[type].direct = 1;
        }
        else
            return yieldError(labelNotExist);
    }
    return True;
}

Bool writeDataInstruction(char *token)
{
    int num;
    while (token != NULL)
    {
        num = atoi(token);
        addWord(num, Data);
        token = strtok(NULL, ", \t\n\f\r");
    }
    return True;
}

Bool writeStringInstruction(char *s)
{
    char *start = strchr(s, '\"');
    int i, len;
    start++;
    len = strlen(start);
    for (i = 0; i < len - 1; i++)
        addWord(start[i], Data);

    addWord('\0', Data);
    return True;
}

int parseRegNumberFromOperand(char *s)
{
    s = strchr(s, 'r');
    return getRegisteryNumber(s);
}
