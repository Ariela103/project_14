#include "data.h"

char *cloneString(char *s)
{
    char *copy;
    if (!(*s))
        return "";
    copy = (char *)calloc(strlen(s), sizeof(char *));
    strcpy(copy, s);
    return copy;
}
char *trimFromLeft(char *s)
{
    while (isspace(*s) && *s != '\0')
        s++;
    return s;
}


char *decToOctal(int num)
{
    int i = num, size = 0;
    char *octal;

    for (size = 0; i > 0; i = i / 8)
        size++;

    octal = (char *)calloc(size + 1, sizeof(char));
    sprintf(octal, "%0*o", size, num);

    return octal;
}

char *numToBin(int num)
{
    /* Allocate memory for the binary string (15 characters + null terminator) */
    int i = 0;
    char *binaryString = (char *)calloc(BINARY_WORD_SIZE + 1, sizeof(char));

    /* Mask the number to fit within 15 bits (two's complement) */
    unsigned int maskedNum = num & ((1 << BINARY_WORD_SIZE) - 1); /* Only keep the last 15 bits */

    /* Convert the masked number to binary string (from right to left) */
    for (i = BINARY_WORD_SIZE - 1; i >= 0; i--)
    {
        binaryString[i] = (maskedNum & 1) ? '1' : '0'; /*Get the last bit*/
        maskedNum >>= 1;                               /*Shift right*/
    }

    return binaryString;
}


OctalWord *convertBinaryWordToOctal(BinaryWord *word)
{
    int i = 0;
    char octalDigits[4] = {0,0,0,'\0'};
    OctalWord *newOctal = (OctalWord *)malloc(sizeof(OctalWord));

    for (i = BINARY_WORD_SIZE - 1; i >= 0; i--)
    {
        octalDigits[i % 3] = word->digit[i].on ? '1' : '0';
        /* Once we've filled 3 binary digits, convert them to octal*/
        if (i % 3 == 0)
        {
            /* Calculate the octal number from the 3 binary digits */
            char octalValue = binaryStringToOctalNumber(octalDigits);
            /* Determine which field of newOctal to store the octal value */
            switch (i)
            {
            case 12:
                newOctal->_E = octalValue;
                break;
            case 9:
                newOctal->_D = octalValue;
                break;
            case 6:
                newOctal->_C = octalValue;
                break;
            case 3:
                newOctal->_B = octalValue;
                break;
            case 0:
                newOctal->_A = octalValue;
                break;
            default:
                break;
            }

            /* Reset the octalDigits array */
            memset(octalDigits, 0, 3);
        }
    }

    return newOctal;
}

unsigned binaryStringToOctalNumber(char binaryStr[3])
{
    if (!strcmp(binaryStr, "000"))
        return 0;
    if (!strcmp(binaryStr, "001"))
        return 1;
    if (!strcmp(binaryStr, "010"))
        return 2;
    if (!strcmp(binaryStr, "011"))
        return 3;
    if (!strcmp(binaryStr, "100"))
        return 4;
    if (!strcmp(binaryStr, "101"))
        return 5;
    if (!strcmp(binaryStr, "110"))
        return 6;
    if (!strcmp(binaryStr, "111"))
        return 7;

    return 0;
}

unsigned binaryStringToHexNumber(char binaryStr[4])
{

    if (!strcmp(binaryStr, "0000"))
        return 0;
    if (!strcmp(binaryStr, "0001"))
        return 1;
    if (!strcmp(binaryStr, "0010"))
        return 2;
    if (!strcmp(binaryStr, "0011"))
        return 3;
    if (!strcmp(binaryStr, "0100"))
        return 4;
    if (!strcmp(binaryStr, "0101"))
        return 5;
    if (!strcmp(binaryStr, "0110"))
        return 6;
    if (!strcmp(binaryStr, "0111"))
        return 7;
    if (!strcmp(binaryStr, "1000"))
        return 8;
    if (!strcmp(binaryStr, "1001"))
        return 9;
    if (!strcmp(binaryStr, "1010"))
        return 0xA;
    if (!strcmp(binaryStr, "1011"))
        return 0xB;
    if (!strcmp(binaryStr, "1100"))
        return 0xC;
    if (!strcmp(binaryStr, "1101"))
        return 0xD;
    if (!strcmp(binaryStr, "1110"))
        return 0xE;
    if (!strcmp(binaryStr, "1111"))
        return 0xF;

    return 0;
}
