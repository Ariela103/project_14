#include "data.h"

/* Global variables for binary and octal memory images */
static BinaryWord *binaryImg = NULL;
static OctalWord *OctalImg = NULL;
unsigned static IC = MEMORY_START; /* Instruction Counter */
unsigned static DC = 0;            /* Data Counter */
unsigned static ICF = 0;           /* Instruction Counter Final */
unsigned static DCF = 0;           /* Data Counter Final */

/* External functions declared from other files */
extern OctalWord *convertBinaryWordToOctal(BinaryWord *word);
extern char *numToBin(int num);
extern unsigned binaryStringToOctalNumber(char binaryStr[3]);

/* Getter functions for DC, IC, ICF, and DCF */
unsigned getDC() { return DC; }
unsigned getIC() { return IC; }
unsigned getICF() { return ICF; }
unsigned getDCF() { return DCF; }

/**
 * increaseDataCounter
 * -------------------
 * Increases the data counter (DC) by a specified amount.
 *
 * Parameters:
 * - amount: The amount by which to increase the DC.
 */
void increaseDataCounter(int amount)
{
    DC += amount;
}

/**
 * increaseInstructionCounter
 * --------------------------
 * Increases the instruction counter (IC) by a specified amount.
 *
 * Parameters:
 * - amount: The amount by which to increase the IC.
 */
void increaseInstructionCounter(int amount)
{
    IC += amount;
}

/**
 * allocMemoryImg
 * --------------
 * Allocates memory for the binary and octal memory images. Initializes the binary and octal words.
 * The size of the memory images is based on the DCF (Data Counter Final).
 */
void allocMemoryImg()
{
    const int totalSize = DCF - MEMORY_START; /* Calculate total size based on final data counter */
    int i, j;

    /* Free previously allocated memory, if any */
    if (binaryImg != NULL)
        free(binaryImg);
    if (OctalImg != NULL)
        free(OctalImg);

    /* Allocate memory for binary and octal images */
    binaryImg = (BinaryWord *)malloc(totalSize * sizeof(BinaryWord));
    OctalImg = (OctalWord *)malloc(totalSize * sizeof(OctalWord));

    /* Initialize binary and octal memory with default values */
    for (i = 0; i < totalSize; i++)
    {
        OctalImg[i]._A = 0;
        OctalImg[i]._B = 0;
        OctalImg[i]._C = 0;
        OctalImg[i]._D = 0;
        OctalImg[i]._E = 0;

        for (j = 0; j < BINARY_WORD_SIZE; j++)
        {
            binaryImg[i].digit[j].on = 0;
        }
    }
}

/**
 * resetMemoryCounters
 * -------------------
 * Resets the memory counters (IC, DC, ICF, DCF) to their initial values.
 */
void resetMemoryCounters()
{
    IC = MEMORY_START;
    DC = 0;
    ICF = 0;
    DCF = 0;
}

/**
 * printBinaryImg
 * --------------
 * Prints the binary memory image in a human-readable format, displaying each word in binary.
 */
void printBinaryImg()
{
    int i;
    int totalSize = DCF - MEMORY_START;
    for (i = 0; i < totalSize; i++)
    {
        /* Print the memory address */
        printf("%04d ", MEMORY_START + i);
        /* Print the binary word at the current address */
        printWordBinary(i);
    }
}

/**
 * addWord
 * -------
 * Adds a word to either the code or data image depending on the DataType.
 *
 * Parameters:
 * - value: The integer value to be converted and added.
 * - type: The type (Code or Data) to determine where the word should be added.
 */
void addWord(int value, DataType type)
{
    if (type == Code)
        addWordToCodeImage(numToBin(value));
    else if (type == Data)
        addWordToDataImage(numToBin(value));
}

/**
 * addWordToDataImage
 * ------------------
 * Adds a word to the data image by converting the binary string into a binary word object.
 * Increments the data counter (DC).
 *
 * Parameters:
 * - s: The binary string representing the word.
 */
void addWordToDataImage(char *s)
{
    wordStringToWordObj(s, Data);
    DC++;
}

/**
 * addWordToCodeImage
 * ------------------
 * Adds a word to the code image by converting the binary string into a binary word object.
 * Increments the instruction counter (IC).
 *
 * Parameters:
 * - s: The binary string representing the word.
 */
void addWordToCodeImage(char *s)
{
    wordStringToWordObj(s, Code);
    IC++;
}

/**
 * wordStringToWordObj
 * -------------------
 * Converts a binary string into a BinaryWord object and stores it in the appropriate location
 * (either in the code or data section based on the type).
 *
 * Parameters:
 * - s: The binary string representing the word.
 * - type: The type (Code or Data) determining where to store the binary word.
 */
void wordStringToWordObj(char *s, DataType type)
{
    int j;
    int index = type == Code ? IC - MEMORY_START : DC - MEMORY_START; /* Determine the index based on type */

    /* Convert the binary string into a binary word object */
    for (j = 0; j < BINARY_WORD_SIZE; j++)
        binaryImg[index].digit[j].on = s[j] == '1' ? 1 : 0;
}

/**
 * printWordBinary
 * ---------------
 * Prints a binary word at a specific index in the binary memory image.
 *
 * Parameters:
 * - index: The index of the binary word to be printed.
 */
void printWordBinary(unsigned index)
{
    int j;
    for (j = 0; j < BINARY_WORD_SIZE; j++)
    {
        if (j % 3 == 0)
            printf(" ");
        printf("%d", binaryImg[index].digit[j].on ? 1 : 0);
    }

    printf("\n");
}

/**
 * calcFinalAddrsCountersValues
 * ----------------------------
 * Calculates the final values of the instruction counter (ICF) and data counter (DCF) after the second pass.
 * Adjusts the data counter to follow the instructions and resets the instruction counter.
 */
void calcFinalAddrsCountersValues()
{
    ICF = IC;          /* Set the final instruction counter */
    DCF = ICF + DC;    /* Calculate the final data counter */
    DC = IC;           /* Adjust the data counter to follow the instructions */
    IC = MEMORY_START; /* Reset the instruction counter to the memory start */
}

/**
 * printMemoryImgInRequiredObjFileFormat
 * -------------------------------------
 * Prints the memory image (both binary and octal representations) in the format required for the object file.
 */
void printMemoryImgInRequiredObjFileFormat()
{
    extern BinaryWord *binaryImg;
    extern OctalWord *OctalImg;
    int i;
    int totalSize = DCF - MEMORY_START;

    /* Print the size of the code and data sections */
    printf("%d %d\n", ICF - MEMORY_START, DCF - ICF);

    /* Convert and print each word in octal format */
    for (i = 0; i < totalSize; i++)
    {
        OctalImg[i] = *convertBinaryWordToOctal(&binaryImg[i]);
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", MEMORY_START + i, OctalImg[i]._A, OctalImg[i]._B, OctalImg[i]._C, OctalImg[i]._D, OctalImg[i]._E);
    }
}

/**
 * writeMemoryImageToObFile
 * ------------------------
 * Writes the memory image (binary and octal representations) to the object file in the required format.
 *
 * Parameters:
 * - fp: The file pointer to the object file.
 */
void writeMemoryImageToObFile(FILE *fp)
{
    extern BinaryWord *binaryImg;
    extern OctalWord *OctalImg;
    int i;
    int totalSize = DCF - MEMORY_START;

    /* Write the size of the code and data sections */
    fprintf(fp, "%d %d\n", ICF - MEMORY_START, DCF - ICF);

    /* Convert and write each word in octal format to the file */
    for (i = 0; i < totalSize; i++)
    {
        OctalImg[i] = *convertBinaryWordToOctal(&binaryImg[i]);
        fprintf(fp, "%04d %x%x%x%x%x\n", MEMORY_START + i, OctalImg[i]._A, OctalImg[i]._B, OctalImg[i]._C, OctalImg[i]._D, OctalImg[i]._E);
    }
}
