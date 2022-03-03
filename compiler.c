#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern void parseSourceFile(FILE *source, char *filename);
extern int parseExpandedSourceFile(FILE *fp, char *filename);
/*
run program on test code, code from file code-for-test.as
sh test.sh
 */
int main(int argc, char *argv[])
{

    handleSourceFiles(argc, argv);
    globalState = firstRun;
    printf("Start Parsing Assembly Code:\n");
    handleSourceFiles(argc, argv);

    if (globalState != collectErrors)
        printf("Finished Successfully!\n");
    else
        printf("\nFinished First Run With Errors\n");

    printSymbolTable();

    /*
    symbol table from test should include:
    x data entry
    y data
    Bla external
    dsafs external
    str2 string
    str string
    */

    return 0;
}

int handleSourceFiles(int argc, char *argv[])
{
    FILE *fptr;
    char *fileName;
    int filesCount = argc - 1;
    int i = 1;
    if (filesCount < 1)
    {
        yieldError(AssemblerDidNotGetSourceFiles);
        exit(1);
    }

    while (--argc)
    {
        fileName = calloc(strlen(argv[i]) + 3, sizeof(char *));
        sscanf(argv[i], "%s", fileName);
        fileName = globalState == handleMacros ? strcat(fileName, ".as") : strcat(fileName, ".am");

        if ((fptr = fopen(fileName, "r")) == NULL)
            yieldError(fileCouldNotBeOpened);
        else
        {
            if (globalState == handleMacros)
                parseSourceFile(fptr, fileName);
            if (globalState == firstRun)
                parseExpandedSourceFile(fptr, fileName);

            fclose(fptr);
            free(fileName);
        }
    }

    return True;
}
