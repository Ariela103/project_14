#include "data.h"

extern void fileCreationFailure(char *fileName);
static char *(*baseFileName)() = &getFileNamePath;

/*  void exportFiles()
    This function is responsible for exporting the .ob, .ent, and .ext files.
    It creates the .ob file first, then generates the .ent file if any entry labels exist,
    and the .ext file if any external labels exist.*/

void exportFiles()
{
    printf("Finished Successfully, about to export files!\n");
    createObFile();
    if (areEntriesExist())
        createEntriesFile();
    if (areExternalsExist())
        createExternalsFile();
}

/* void createObFile()
    This function creates the .ob file by writing the memory image.
    If the file can't be created, an error is logged. */
void createObFile()
{
    FILE *ob;
    char *fileName = (*baseFileName)();
    strcat(fileName, ".ob");
    ob = fopen(fileName, "w+");

    if (ob != NULL)
    {
        writeMemoryImageToObFile(ob);
        fclose(ob);
        free(fileName);
    }
    else
        fileCreationFailure(fileName);
}

/* void createEntriesFile()
    This function generates the .ent file if there are entry labels by writing
    the necessary data. If the file can't be created, an error is logged. */
void createEntriesFile()
{
    FILE *ent;
    char *fileName = (*baseFileName)();
    strcat(fileName, ".ent");
    ent = fopen(fileName, "w+");

    if (ent != NULL)
    {
        writeEntriesToFile(ent);
        fclose(ent);
        free(fileName);
    }
    else
        fileCreationFailure(fileName);
}

/* void createExternalsFile()
    This function generates the .ext file if there are external labels.
    It writes the data to the file. If the file can't be created, an error is logged. */
void createExternalsFile()
{
    FILE *ext;
    char *fileName = (*baseFileName)();
    strcat(fileName, ".ext");
    ext = fopen(fileName, "w+");
    if (ext != NULL)
    {
        writeExternalsToFile(ext);
        fclose(ext);
        free(fileName);
    }
    else
        fileCreationFailure(fileName);
}
