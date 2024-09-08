
/*
-----------------------------------------------------------------------------------------
--------------------------- mem.h General Overview: ---------------------------
-----------------------------------------------------------------------------------------

This module is responsible for managing the memory image during the assembly process.
It handles memory allocation, increases and resets data and instruction counters,
and builds the binary or hexadecimal memory image word by word.
This module is utilized during both the first and second passes of the assembler.

During the first pass, functions here are used to increment or decrement the data and instruction counters (DC/IC).
In the second pass, the functions build the final memory image, which is later exported.

-----------------------------------------------------------------------------------------
*/

/**
 * @brief Increases the data counter by the specified amount.
 *
 * This function increments the data counter (DC) by the given number of memory addresses.
 *
 * @param amount The amount to increase the data counter by.
 */
void increaseDataCounter(int amount);

/**
 * @brief Increases the instruction counter by the specified amount.
 *
 * This function increments the instruction counter (IC) by the given number of memory addresses.
 *
 * @param amount The amount to increase the instruction counter by.
 */
void increaseInstructionCounter(int amount);

/**
 * @brief Allocates memory for the final memory image.
 *
 * This function allocates the exact amount of space needed for the compiled memory image
 * based on the calculated size of the data and instruction segments.
 */
void allocMemoryImg();

/**
 * @brief Resets the data and instruction counters.
 *
 * This function resets both the data counter (DC) and instruction counter (IC)
 * to prepare for a fresh run of memory allocation and processing.
 */
void resetMemoryCounters();

/**
 * @brief Converts a binary word string to a `BinaryWord` structure.
 *
 * This function takes a string representation of a binary word and converts it into
 * a `BinaryWord` structure. The data type parameter specifies whether the word belongs
 * to the instruction or data part of the memory image.
 *
 * @param s The binary word string to convert.
 * @param type The data type (instruction or data).
 */
void wordStringToWordObj(char *s, DataType type);

/**
 * @brief Writes the memory image to the specified object (.ob) file.
 *
 * This function generates the .ob memory image by writing each word in the memory image
 * (code and data) to the specified file in the required format.
 *
 * @param fp The file pointer to the .ob file.
 */
void writeMemoryImageToObFile(FILE *fp);

/**
 * @brief Prints the memory image in the required object file format.
 *
 * This function formats and prints the memory image (binary or hexadecimal) for debugging purposes.
 */
void printMemoryImgInRequiredObjFileFormat();

/**
 * @brief Calculates the final values for the data and instruction counters.
 *
 * This function finalizes the values of the data counter (DC) and instruction counter (IC)
 * after the second pass of the assembler.
 */
void calcFinalAddrsCountersValues();

/**
 * @brief Prints the binary representation of a word at the specified index.
 *
 * @param index The index of the word in the memory image to print.
 */
void printWordBinary(unsigned index);

/**
 * @brief Adds a word to the code segment of the memory image.
 *
 * This function adds a word to the code image by converting a string representation
 * of the word and appending it to the code image.
 *
 * @param s The string representing the word to add.
 */
void addWordToCodeImage(char *s);

/**
 * @brief Adds a word to the data segment of the memory image.
 *
 * This function adds a word to the data image by converting a string representation
 * of the word and appending it to the data image.
 *
 * @param s The string representing the word to add.
 */
void addWordToDataImage(char *s);

/**
 * @brief Adds a word to either the code or data image based on the data type.
 *
 * This function adds a word to the memory image, either the code segment or data segment,
 * depending on the specified data type.
 *
 * @param value The value of the word to add.
 * @param type The data type (instruction or data).
 */
void addWord(int value, DataType type);

/**
 * @brief Prints the complete binary memory image.
 *
 * This function prints the entire memory image in binary format for debugging purposes.
 */
void printBinaryImg();

/**
 * @brief Retrieves the current value of the data counter (DC).
 *
 * @return unsigned The current value of the data counter.
 */
unsigned getDC();

/**
 * @brief Retrieves the current value of the instruction counter (IC).
 *
 * @return unsigned The current value of the instruction counter.
 */
unsigned getIC();

/**
 * @brief Retrieves the final value of the instruction counter (ICF) after the second pass.
 *
 * @return unsigned The final value of the instruction counter after processing.
 */
unsigned getICF();

/**
 * @brief Retrieves the final value of the data counter (DCF) after the second pass.
 *
 * @return unsigned The final value of the data counter after processing.
 */
unsigned getDCF();
