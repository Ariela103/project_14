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


/*
char *decToHex(int num)
{
    int i = num, size = 0;
    char *hex;
    for (size = 0; i > 0; i = i / 16)
        size++;
    hex = (char *)calloc(size, sizeof(char *));
    sprintf(hex, "%05x", num);
    return hex;
}
*/
char *decToOctal(int num) {
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
    int i = 0;
    unsigned int result;
    char *word, hex[6];
    word = (char *)calloc(BINARY_WORD_SIZE + 1, sizeof(char *));
    if (num < 0)
    {
        result = abs(num);
        result = ~result;
        result++;
        sprintf(hex, "%05x", (int)(result & 0x4ffff));
    }
    else
        sprintf(hex, "%05x", (int)num & 0xfffff);

    while (hex[i] != '\0')
    {
        switch (hex[i])
        {

        case '0':
            strcat(word, "0000");
            break;
        case '1':
            strcat(word, "0001");
            break;
        case '2':
            strcat(word, "0010");
            break;
        case '3':
            strcat(word, "0011");
            break;
        case '4':
            strcat(word, "0100");
            break;
        case '5':
            strcat(word, "0101");
            break;
        case '6':
            strcat(word, "0110");
            break;
        case '7':
            strcat(word, "0111");
            break;
        case '8':
            strcat(word, "1000");
            break;
        case '9':
            strcat(word, "1001");
            break;
        case 'A':
        case 'a':
            strcat(word, "1010");
            break;
        case 'B':
        case 'b':
            strcat(word, "1011");
            break;
        case 'C':
        case 'c':
            strcat(word, "1100");
            break;
        case 'D':
        case 'd':
            strcat(word, "1101");
            break;
        case 'E':
        case 'e':
            strcat(word, "1110");
            break;
        case 'F':
        case 'f':
            strcat(word, "1111");
            break;
        default:
            break;
        }

        i++;
    }

    strcat(word, "\0");
    return word;
}
/*
HexWord *convertBinaryWordToHex(BinaryWord *word)
{
    int i = 0;
    char hexDigits[4] = {0};
    HexWord *newHex = (HexWord *)malloc(sizeof(HexWord));
    for (i = BINARY_WORD_SIZE - 1; i >= 0; i--)
    {
        hexDigits[i % 4] = word->digit[i].on ? '1' : '0';
        if (i % 4 == 0)
        {
            switch (i)
            {
            case 16:
                newHex->_E = binaryStringToHexNumber(hexDigits);
                break;
            case 12:
                newHex->_D = binaryStringToHexNumber(hexDigits);
                break;
            case 8:
                newHex->_C = binaryStringToHexNumber(hexDigits);
                break;
            case 4:
                newHex->_B = binaryStringToHexNumber(hexDigits);
                break;
            case 0:
                newHex->_A = binaryStringToHexNumber(hexDigits);
                break;
            default:
                break;
            }

            memset(hexDigits, 0, 4);
        }
    }

    return newHex;
}
*/

OctalWord *convertBinaryWordToOctal(BinaryWord *word) {
    int i = 0;
    char octalDigits[3] = {0};
    OctalWord *newOctal = (OctalWord *)malloc(sizeof(OctalWord));

    for (i = BINARY_WORD_SIZE - 1; i >= 0; i--) {
        octalDigits[i % 3] = word->digit[i].on ? '1' : '0';
        
        /* Once we've filled 3 binary digits, convert them to octal*/
        if (i % 3 == 0) {
            /* Calculate the octal number from the 3 binary digits */
            char octalValue = binaryStringToOctalNumber(octalDigits);

            /* Determine which field of newOctal to store the octal value */
            switch (i / 3) {
                case 4:
                    newOctal->_A = octalValue;
                    break;
                case 3:
                    newOctal->_B = octalValue;
                    break;
                case 2:
                    newOctal->_C = octalValue;
                    break;
                case 1:
                    newOctal->_D = octalValue;
                    break;
                case 0:
                    newOctal->_E = octalValue;
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

unsigned binaryStringToOctalNumber(char binaryStr[3]) {

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

