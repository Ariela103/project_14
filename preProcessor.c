

#include "data.h"
extern Bool isLabelDeclaration(char *s);
extern Bool isOperation(char *s);
extern Bool isComment(char *s);
extern Bool isInstructionStrict(char *s);
extern Bool verifyLabelNaming(char *s);
extern Item *addMacro(char *name, int start, int end);
extern Item *updateMacro(char *name, int start, int end);
extern Item *getMacro(char *s);

extern void printMacroTable();
extern int printMacroItem(Item *item);
extern unsigned currentLineNumber;
extern void updateGlobalState(State new);

extern Bool isPossiblyUseOfMacro(char *s);
extern Bool isMacroOpening(char *s);
extern Bool isMacroClosing(char *s);
extern Bool isLegalMacroName(char *s);

void saveMacros(FILE *target);
void popLastToken(FILE *target, char *token, int offset);
void parseAndReplaceMacros(FILE *source, FILE *target);
void replaceWithMacro(FILE *target, FILE *source, int start, int end);
void createExpandedSourceFile(FILE *source, FILE *target, char *fileName);
void popCharacters(FILE *target, fpos_t position, int amount);

void parseAndReplaceMacros(FILE *source, FILE *target)
{
    ParseState state = evalToken;
    Bool isMacroCurrentlyParsed = False;
    Bool isMacroStartFoundYet = False;
    int c = 0, i = 0, j = 0, offsetCounter = 0, current = 0;
    fpos_t position;
    int start = 0, end = 0;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    fgetpos(target, &position);
    while ((c = fgetc(source)) != EOF)
    {

        if (!isMacroCurrentlyParsed)
        {
            fputc(c, target);
            offsetCounter++;
        }
        if (isMacroCurrentlyParsed && isMacroStartFoundYet)
            end++;
        if (c == '\n')
        {
            offsetCounter = 0;
            currentLineNumber++;
            if (state == skipLine)
                state = evalToken;
        }

        if (state != skipLine)
        {
            if (isMacroCurrentlyParsed && !isMacroStartFoundYet)
            {
                start = ftell(source) - 1;
                isMacroStartFoundYet = True;
            }

            if (!isspace(c))
            {

                if (j < MAX_LABEL_LEN && i < MAX_LABEL_LEN)
                {

                    if (state == parsingMacroName)
                        macroName[i++] = c;

                    else
                        token[j++] = c;
                }
            }
            else if (j > 0 || i > 0)
            {

                if (state == evalToken)
                {

                    if (isMacroOpening(token))
                    {
                        popLastToken(target, token, 0);
                        state = parsingMacroName;
                        isMacroStartFoundYet = False;
                    }
                    else
                    {
                        if (isMacroClosing(token))
                        {
                            /*                                 printf("\nisMacroClosing\ntoken:%s\n", token);
                             */
                            popLastToken(target, token, 0);
                            addMacro(macroName, start, (ftell(source) - strlen(token) - 1));
                            memset(macroName, 0, MAX_LABEL_LEN);
                            isMacroCurrentlyParsed = False;
                            i = start = end = 0;
                        }

                        else if (isPossiblyUseOfMacro(token))
                        {
                            Item *p = getMacro(token);
                            popLastToken(target, token, 0);

                            if (p != NULL)
                            {
                                current = ftell(source) - 1;
                                fseek(target, -strlen(token), SEEK_CUR);
                                replaceWithMacro(target, source, p->val.m.start, p->val.m.end);
                                fseek(source, current, SEEK_SET);
                            }
                        }

                        state = skipLine;
                    }
                }

                else if (state == parsingMacroName)
                {
                    popLastToken(target, macroName, 0);

                    if (isLegalMacroName(macroName))
                    {

                        isMacroCurrentlyParsed = True;
                        state = evalToken;
                    }
                    else
                    {
                        yieldError(illegalMacroNameUseOfSavedKeywords);
                        memset(macroName, 0, MAX_LABEL_LEN);
                        isMacroCurrentlyParsed = isMacroStartFoundYet = False;
                        state = skipLine;
                    }
                }
                memset(token, 0, MAX_LABEL_LEN);
                i = j = 0;
            }
        }
    }

    if (offsetCounter && (state == evalToken || state == parsingMacroName))
    {
        if (isMacroOpening(token) || isMacroClosing(token))
        {
            popLastToken(target, "", offsetCounter - j - i);
        }

        else if (isPossiblyUseOfMacro(token))
        {
            Item *p = getMacro(token);
            popLastToken(target, token, offsetCounter - j - i);
            if (p != NULL)
                replaceWithMacro(target, source, p->val.m.start, p->val.m.end);
        }
    }
}
void popCharacters(FILE *target, fpos_t position, int amount)
{ /*
    int c = 0;
        fsetpos(target, &position);
        while (amount-- && (c = fgetc(target)) != EOF)
        {

            printf("line 184\n");
            fputc(' ', target);
        }
     */
}

void popLastToken(FILE *target, char *token, int offset)
{

    int len = strlen(token);
    /*     printf("\n\ninside popLastToken,\nline:%d\ntoken:%s\nlen:%d\n", currentLineNumber, token, len);
     */
    fseek(target, -len, SEEK_CUR);
    fputc(' ', target);
    if (offset)
    {
        fseek(target, -offset, SEEK_CUR);
        while (--offset)
            fputc(' ', target);
    }
}
void replaceWithMacro(FILE *target, FILE *source, int start, int end)
{
    int c, i;
    fseek(source, start, SEEK_SET);
    fseek(target, 0, SEEK_CUR);
    for (i = end - start - 1; i > 0 && (c = fgetc(source)) != EOF; i--)
        fputc(c, target);
}

void createExpandedSourceFile(FILE *source, FILE *target, char *fileName)
{

    parseAndReplaceMacros(source, target);
    rewind(target);
    fclose(source);
    updateGlobalState(firstRun);
}