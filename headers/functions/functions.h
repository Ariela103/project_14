/* FUNCTIONS THAT ARE COMMENTED OUT MEANS THAT THEY ARE NOT SHARED ACROSS FILES */
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In globalVariables.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void setGlobalState(State new);
State getGlobalState();

void setCurrentFileName(char *s);
char *getCurrentFileName();

int getCurrentLineNumber();
void increaseCurrentLineNumber();
void resetCurrentLineNumber();

void setFileNamePath(char *s);
char *getFileNamePath();
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In exportFiles.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
void generateObFile();
void createEntriesFile();
void createExternalsFile();
extern char *getFileNamePath();

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In memory.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
int writeSingleEntry(Item *item, FILE *fp, int count);
void writeEntriesToFile(FILE *fp);
void writeMemoryImageToObFile(FILE *fp);
void increaseDataCounter(int amount);
void increaseInstructionCounter(int amount);
void updateFinalCountersValue();
unsigned getDC();
unsigned getIC();
unsigned getICF();
unsigned getDCF();
void initMemory();
void printBinaryImg();
void printWordBinary(unsigned index);
void wordStringToWordObj(char *s, DataType type);
void addWordToDataImage(char *s);
void addWordToCodeImage(char *s);
void addWord(int value, DataType type);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In compiler.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

int handleSourceFiles(int argc, char *argv[]);
void parseAssemblyCode(FILE *src);
ParseState parseLine(char *token, char *line);
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In preProcessor.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void parseSourceFile(FILE *src, FILE *target);
Bool parseMacros(char *line, char *token, FILE *src, FILE *target);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In errors.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

Bool yieldError(Error err);
Bool yieldWarning(Warning err);
void yieldWarningIntoFile(Warning err);
void yieldErrorIntoFile(Error err);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In Operations.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

Operation *getOperationByName(char *s);
Operation *getOperationByIndex(unsigned int i);
int getOpIndex(char *s);
Bool isOperationName(char *s);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In table.c --------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void printMacroTable();
int printMacroItem(Item *item);
unsigned hash(char *s);
Item *lookup(char *s, ItemType type);
Item *install(char *name, ItemType type);
void printSymbolTable();
int printSymbolItem(Item *item);
Item *getSymbol(char *name);
Item *addMacro(char *name, int start, int end);
Bool addSymbol(char *name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
Bool updateSymbol(Item *p, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
Item *updateSymbolAddressValue(char *name, int newValue);
Item *getMacro(char *s);
Item *updateMacro(char *name, int start, int end);
Bool verifyLabelNaming(char *s);
Item *removeFromTable(char *name, ItemType type);
Bool verifyLabelNamingAndPrintErrors(char *s);
Bool isLabelNameAlreadyTaken(char *name, ItemType type);
void initTables();
void updateFinalSymbolTableValuesAndCountEntriesAndExternals();
int updateFinalValueOfSingleItem(Item *item);
int getSymbolBaseAddress(char *name);
int getSymbolOffset(char *name);
Bool isExternal(char *name);
Bool isEntry(char *name);
Bool isNonEmptyEntry(char *name);

Bool areEntriesExist();
Bool areExternalsExist();

void writeExternalsToFile(FILE *fp);
void writeSingleExternal(FILE *fp, char *name, ExtPositionData *value);

void initExternalOperandsList();
ExtListItem *findExternalOperandListItem(char *name);
int findExternalOperandListItemIndex(char *name);
void updateExternalOperandList(char *name, unsigned base, unsigned offset);
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In encode.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void printObjectFile(HexWord *words[], unsigned int ICF, unsigned int DCF);
void printBinaryFile(HexWord *words[], unsigned int ICF, unsigned int DCF);
HexWord *convertNumToHexWord(int num);
BinaryWord *convertNumberToBinaryWord(int num);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In helpers.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
unsigned binaryStringToHexNumber(char binaryStr[4]);
char *decToHex(int num);
char *hexToBin(char *hex);
int hex2int(char ch);
int countSpaceCharacters(char *s);
char *numToBin(int num);
int countConsecutiveCommas(char *s);
int countLengthOfNonDigitToken(char *s);
char *cloneString(char *s);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In firstRun.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
int firstRunParsing(FILE *fp, char *filename);
Bool isInstruction(char *s);
Bool isInstructionStrict(char *s);
Bool handleSingleLine(char *line);
ParseState handleFirstToken(char *token, char *line, ParseState state);
ParseState handleOperation(char *operationName, char *args);
Bool parseOperands(char *src, char comma, char *des, Operation *op, AddrMethodsOptions active[2]);
Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, AddrMethodsOptions active[2], char *operand, int type);
ParseState handleInstruction(int type, char *firstToken, char *nextTokens, char *line);
ParseState handleLabel(char *labelName, char *nextToken, char *line);
Bool isLabelDeclaration(char *s);
Bool isOperation(char *s);
int getInstructionType(char *s);
char *getInstructionName(char *s);
char *getInstructionNameByType(int type);
Bool countAndVerifyStringArguments(char *tokens);
Bool isRegistery(char *s);
Bool isValidImmediateParamter(char *s);
int getRegisteryNumber(char *s);
Bool isValidIndexParameter(char *s);

Bool isComment(char *s);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In parse.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
Bool countAndVerifyDataArguments(char *line);
Bool countAndVerifyStringArguments(char *token);
char *trimFromLeft(char *s);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In Second Run.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

int secondRunParsing(FILE *fp, char *filename);
void writeDirectOperandWord(char *labelName);
void writeFirstWord(Operation *operation);
void writeSecondWord();
Bool writeOperationBinary(char *operationName, char *line);
Bool writeInstructionBinary(char *instructionName, char *line);
void parseSingleLinesecondRunParsing(char *line);
ParseState parseLine(char *token, char *line);
Bool detectOperandType(char *operand, AddrMethodsOptions active[2], int type);
void writeSecondWord(char *first, char *second, AddrMethodsOptions active[2], Operation *op);
void writeFirstWord(Operation *op);
void writeImmediateOperandWord(char *n);
char *parseLabelNameFromIndexAddrOperand(char *s);
int parseRegNumberFromIndexAddrOperand(char *s);
Bool writeStringInstruction(char *s);
Bool writeDataInstruction(char *s);
void writeSingleExternalsFile(char *name, unsigned base, unsigned offset);
