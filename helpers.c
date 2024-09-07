#include "data.h"

/**
 * cloneString
 * ------
 * This function creates a duplicate of the input string.
 *
 * Parameters:
 * - s: The string to be cloned.
 *
 * Returns:
 * - A pointer to the newly allocated string that is a copy of the input string.
 * - If the input string is empty, it returns an empty string.
 */
char *cloneString(char *s)
{
    char *copy;
    if (!(*s))
        return ""; /* Return empty string if input is empty */

    /* Allocate memory for the copy and copy the string */
    copy = (char *)calloc(strlen(s), sizeof(char *));
    strcpy(copy, s);
    return copy;
}

/**
 * trimFromLeft
 * ------
 * This function removes any leading whitespace characters from the input string.
 *
 * Parameters:
 * - s: The input string from which leading whitespace is to be removed.
 *
 * Returns:
 * - A pointer to the first non-whitespace character in the string.
 */
char *trimFromLeft(char *s)
{
    /* Move the pointer past any leading whitespace characters */
    while (isspace(*s) && *s != '\0')
        s++;
    return s;
}

/**
 * decToOctal
 * -----
 * This function converts a decimal integer to its octal representation.
 *
 * Parameters:
 * - num: The decimal number to be converted.
 *
 * Returns:
 * - A pointer to a string containing the octal representation of the number.
 */
char *decToOctal(int num)
{
    int i = num, size = 0;
    char *octal;

    /* Determine how many digits are needed for the octal representation */
    for (size = 0; i > 0; i = i / 8)
        size++;

    /* Allocate memory for the octal string and convert the number */
    octal = (char *)calloc(size + 1, sizeof(char));
    sprintf(octal, "%0*o", size, num); /* Convert the number to octal */

    return octal;
}

/**
 * numToBin
 * ----
 * This function converts a decimal number to its binary representation as a 15-bit string.
 *
 * Parameters:
 * - num: The decimal number to be converted.
 *
 * Returns:
 * - A pointer to a string containing the 15-bit binary representation of the number.
 */
char *numToBin(int num)
{
    /* Allocate memory for the binary string (15 characters + null terminator) */
    int i = 0;
    char *binaryString = (char *)calloc(BINARY_WORD_SIZE + 1, sizeof(char));

    /* Mask the number to fit within 15 bits (two's complement) */
    unsigned int maskedNum = num & ((1 << BINARY_WORD_SIZE) - 1); /* Keep only the last 15 bits */

    /* Convert the masked number to a binary string (from right to left) */
    for (i = BINARY_WORD_SIZE - 1; i >= 0; i--)
    {
        binaryString[i] = (maskedNum & 1) ? '1' : '0'; /* Get the last bit */
        maskedNum >>= 1;                               /* Shift right */
    }

    return binaryString;
}

/**
 * convertBinaryWordToOctal
 * ------------
 * This function converts a 15-bit binary word to its octal equivalent, broken into groups of 3 bits.
 *
 * Parameters:
 * - word: A pointer to a BinaryWord structure representing the binary digits.
 *
 * Returns:
 * - A pointer to an OctalWord structure containing the octal representation of the binary word.
 */
OctalWord *convertBinaryWordToOctal(BinaryWord *word)
{
    int i = 0;
    char octalDigits[4] = {0, 0, 0, '\0'};                        /* Temporary storage for 3 binary digits */
    OctalWord *newOctal = (OctalWord *)malloc(sizeof(OctalWord)); /* Allocate memory for the new octal word */

    /* Iterate through the 15-bit binary word from right to left */
    for (i = BINARY_WORD_SIZE - 1; i >= 0; i--)
    {
        /* Store the current binary digit as either '1' or '0' */
        octalDigits[i % 3] = word->digit[i].on ? '1' : '0';

        /* Once we've filled 3 binary digits, convert them to an octal digit */
        if (i % 3 == 0)
        {
            /* Convert the 3 binary digits to an octal number */
            char octalValue = binaryStringToOctalNumber(octalDigits);

            /* Store the octal value in the appropriate field of the OctalWord */
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

            /* Reset the octalDigits array for the next group of 3 binary digits */
            memset(octalDigits, 0, 3);
        }
    }

    return newOctal;
}

/**
 * binaryStringToOctalNumber
 * -------------
 * This function converts a 3-bit binary string to its octal equivalent.
 *
 * Parameters:
 * - binaryStr: A 3-character binary string (e.g., "101").
 *
 * Returns:
 * - An unsigned integer representing the octal value of the binary string.
 */
unsigned binaryStringToOctalNumber(char binaryStr[3])
{
    /* Compare the binary string to known 3-bit values and return the corresponding octal number */
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
