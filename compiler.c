#include "data.h"
void resetMemoryCounters();
void initTables();
void exportFiles();
void closeOpenLogFiles();
void allocMemoryImg();
void calcFinalAddrsCountersValues();
void freeHashTable(ItemType type);

int main(int argc, char *argv[])
{
    /*
     * Passes the command-line arguments (the source files provided) to the
     * handleSourceFiles function for further processing.
     */
    handleSourceFiles(argc, argv);
    return 0;
}

int handleSourceFiles(int argc, char *argv[])
{
    int filesCount = argc - 1;
    int i = 1;

    if (filesCount < 1)
    {
        fprintf(stderr, "\n\nNo source files were provided for the assembler!\n\n");
        exit(1);
    }

    /* Iterates over each file provided as a command-line argument */
    while (--argc)
    {
        handleSingleFile(argv[i]);
        i++;
    }

    return 0;
}

void handleSingleFile(char *arg)
{
    FILE *src = NULL, *target = NULL;
    void (*setPath)(char *) = &setFileNamePath;
    State (*globalState)() = &getGlobalState;
    void (*setState)(State) = &setGlobalState;
    char *fileName = (char *)calloc(strlen(arg) + 4, sizeof(char *));

    strncpy(fileName, arg, strlen(arg)); /* Assigns the base file name */
    strcat(fileName, ".as");             /* Converts it to a .as file */
    (*setPath)(fileName);

    /* If the source file can't be opened, alert the user */
    if ((src = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "\n######################################################################\n");
        fprintf(stderr, " ERROR: Could not open source file %s\n", fileName);
        fprintf(stderr, "######################################################################\n\n");
        free(fileName);
        return;
    }

    /* Modify the file extension from .as to .am */
    fileName[strlen(fileName) - 1] = 'm';
    (*setPath)(fileName);

    /* If the expanded source file cannot be created, notify the user */
    if ((target = fopen(fileName, "w+")) == NULL)
    {
        fprintf(stderr, "\n######################################################################\n");
        fprintf(stderr, " ERROR: Failed to create expanded source file %s\n", fileName);
        fprintf(stderr, "######################################################################\n\n");
        fclose(src);
        free(fileName);
        return;
    }

    else
    {
        /* Initialize tables for processing */
        initTables();

        /* Begin preprocessing (macro parsing), store them in the macro table and print the results */
        (*setState)(parsingMacros);
        resetMemoryCounters();
        parseSourceFile(src, target);
        printMacroTable();
        freeHashTable(Macro);

        /* Proceed to the first run: checks for errors, calculates memory requirements,
           and parses assembly code */
        if ((*globalState)() == firstRun)
        {
            rewind(target);
            parseAssemblyCode(target);

            /* If no errors were found in the first run, move to the second run */
            if ((*globalState)() == secondRun)
            {
                /* Calculate final address values, build memory image, and parse code again */
                calcFinalAddrsCountersValues();
                updateFinalSymbolTableValues();
                allocMemoryImg();
                printSymbolTable();
                rewind(target);
                parseAssemblyCode(target);

                /* If the second run finishes without errors, generate output files */
                if ((*globalState)() == createOutputFiles)
                {
                    fileName[strlen(fileName) - 3] = '\0'; /* Strip the .am extension */
                    (*setPath)(fileName);
                    exportFiles();
                }
                else
                    printf("\nErrors encountered in second run, output files will not be created.\n");
            }
            else
                printf("\nFirst run completed with errors, skipping second run and file export.\n");

            freeHashTable(Symbol);
        }
        else
            printf("\nMacro expansion failed for file: %s\nMoving on to the next file if present.\n\n", fileName);

        /* Clean up: free memory and close open files */
        free(fileName);
        fclose(src);
        fclose(target);
        closeOpenLogFiles();
    }
}
