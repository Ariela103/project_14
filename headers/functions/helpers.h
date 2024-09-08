
/*
-----------------------------------------------------------------------------------------
--------------------------- helpers.h General Overview: ---------------------------
-----------------------------------------------------------------------------------------
This header file provides utility functions for various string manipulations,
numeric conversions, and binary-to-octal conversions. The functions included
handle tasks such as cloning strings, trimming whitespace, converting decimal
numbers to binary and octal formats, and converting binary words into octal words.
These utilities are essential for processing assembly instructions and generating
output in specific formats, such as binary or octal representations.

Key functionalities provided in this file include:
- String manipulation functions to duplicate and clean up input strings.
- Conversion functions for transforming decimal numbers into binary and octal formats.
- Binary word conversion into octal format, breaking binary sequences into manageable parts.
- Utility functions for working with assembly code representation in various numeric bases.

These utilities are fundamental in ensuring smooth parsing, transformation, and
handling of assembly-level operations where precise representation in binary and octal
is crucial.

-----------------------------------------------------------------------------------------
*/

/**
 * @brief Creates a duplicate of the input string.
 *
 * This function allocates memory and creates a copy of the provided string.
 * If the input string is empty, it returns an empty string.
 *
 * @param s The string to be cloned.
 * @return char* A pointer to the newly allocated string that is a copy of the input string.
 */
char *cloneString(char *s);

/**
 * @brief Removes leading whitespace from the input string.
 *
 * This function adjusts the pointer of the input string to skip over any leading whitespace characters.
 *
 * @param s The input string from which leading whitespace is to be removed.
 * @return char* A pointer to the first non-whitespace character in the string.
 */
char *trimFromLeft(char *s);

/**
 * @brief Converts a decimal integer to its octal representation.
 *
 * This function takes a decimal number and returns its octal equivalent as a string.
 *
 * @param num The decimal number to be converted.
 * @return char* A pointer to a string containing the octal representation of the number.
 */

char *numToBin(int num);

/**
 * @brief Converts a 15-bit binary word to its octal representation.
 *
 * This function converts a 15-bit binary word into an octal word, breaking it into groups of 3 bits
 * and mapping them to corresponding octal digits.
 *
 * @param word A pointer to a BinaryWord structure representing the binary digits.
 * @return OctalWord* A pointer to an OctalWord structure containing the octal representation.
 */
OctalWord *convertBinaryWordToOctal(BinaryWord *word);

/**
 * @brief Converts a 3-bit binary string to its octal equivalent.
 *
 * This function takes a 3-character binary string (e.g., "101") and converts it to the corresponding
 * octal value.
 *
 * @param binaryStr A 3-character binary string (e.g., "101").
 * @return unsigned The octal value of the binary string.
 */
unsigned binaryStringToOctalNumber(char binaryStr[3]);