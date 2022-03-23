#include "data.h"
extern char *cloneString(char *s);

static State globalState = startProgram;
void updateGlobalState(State new)
{
    globalState = new;
}
State getGlobalState()
{
    State current = globalState;
    return current;
}

static char *currentFileName;
static char *path;

void setFileNamePath(char *s)
{
    size_t length;
    if (!(*s))
    {
        /*      memset(path, 0, strlen(path)); */
        return;
    }
    length = strlen(s) - strlen(currentFileName);
    path = (char *)realloc(path, length * sizeof(char *));
    strncpy(path, s, length);
    printf("inside setFileNamePath, path:%s\n", path);
}

char *getFileNamePath(char *s)
{
    if (!(*s))
        return "";
    return cloneString(path);
}

void setCurrentFileName(char *s)
{
    currentFileName = (char *)realloc(currentFileName, strlen(s) * sizeof(char));
    strcpy(currentFileName, cloneString(s));
}

char *getCurrentFileName()
{
    return cloneString(currentFileName);
}

static unsigned currentLineNumber = 1;
void resetCurrentLineNumber()
{
    currentLineNumber = 1;
}
void increaseCurrentLineNumber()
{
    currentLineNumber++;
}
int getCurrentLineNumber()
{
    return currentLineNumber;
}
