/*
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
--------------------------- tables.h General Overview: ---------------------------
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------

The `tables` module manages the symbol and macro tables, which are essential for tracking labels,
macros, and external symbols during assembly. It includes functions for adding, updating, and
retrieving items from these tables. Additionally, the module tracks and manages external symbols
and entry symbols, and allows writing the external and entry symbols to respective output files.

The module also provides hash-based storage for efficient lookup and ensures correct memory management
by offering functions to initialize, reset, and free table items. Furthermore, it handles the
external symbols list, allowing updates to positions of symbols and providing functionalities to
export them into files.

-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

/* Function declarations */
void resetExtList();
/* @ Function: resetExtList
   @ Description: Frees all elements in the external list and resets the external count to 0.
*/

ExtListItem *findExtOpListItem(char *name);
/* @ Function: findExtOpListItem
   @ Description: Finds an external list item by its name.
   @ Returns: A pointer to the found external list item or NULL if not found.
*/

void updateExtPositionData(char *name, unsigned base, unsigned offset);
/* @ Function: updateExtPositionData
   @ Description: Updates the base and offset for an external symbol's position.
*/

void freeTableItem(Item *item);
/* @ Function: freeTableItem
   @ Description: Frees a table item and its linked list of items.
*/

void addExtListItem(char *name);
/* @ Function: addExtListItem
   @ Description: Adds a new item to the external list.
*/

unsigned hash(char *s);
/* @ Function: hash
   @ Description: Generates a hash value for a string to use for hashing symbols and macros.
   @ Returns: The hash value of the string.
*/

Item *lookup(char *s, ItemType type);
/* @ Function: lookup
   @ Description: Looks up a symbol or macro by its name.
   @ Returns: Pointer to the found item or NULL if not found.
*/

Item *install(char *name, ItemType type);
/* @ Function: install
   @ Description: Adds a new symbol or macro to the symbol/macro table.
   @ Returns: Pointer to the newly added item or NULL if memory allocation fails.
*/

Bool addSymbol(char *name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
/* @ Function: addSymbol
   @ Description: Adds a new symbol to the symbol table with the given attributes (code, data, entry, external).
   @ Returns: True if the symbol is successfully added, False otherwise.
*/

Bool updateSymbol(Item *p, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
/* @ Function: updateSymbol
   @ Description: Updates an existing symbol in the symbol table.
   @ Returns: True if the symbol is updated successfully, False otherwise.
*/

Item *getSymbol(char *name);
/* @ Function: getSymbol
   @ Description: Looks up and returns a symbol by its name.
   @ Returns: Pointer to the found symbol or NULL if not found.
*/

int getSymbolBaseAddress(char *name);
/* @ Function: getSymbolBaseAddress
   @ Description: Returns the base address of a symbol.
   @ Returns: The base address of the symbol or -1 if not found.
*/

int getSymbolAddress(char *name);
/* @ Function: getSymbolAddress
   @ Description: Returns the value (address) of a symbol.
   @ Returns: The value (address) of the symbol or -1 if not found.
*/

int getSymbolOffset(char *name);
/* @ Function: getSymbolOffset
   @ Description: Returns the offset of a symbol.
   @ Returns: The offset of the symbol or -1 if not found.
*/

Bool isSymbolExist(char *name);
/* @ Function: isSymbolExist
   @ Description: Checks if a symbol exists in the symbol table.
   @ Returns: True if the symbol exists, False otherwise.
*/

Bool isExternal(char *name);
/* @ Function: isExternal
   @ Description: Checks if a symbol is marked as external.
   @ Returns: True if the symbol is external, False otherwise.
*/

Bool isEntry(char *name);
/* @ Function: isEntry
   @ Description: Checks if a symbol is marked as an entry.
   @ Returns: True if the symbol is an entry, False otherwise.
*/

Bool isNonEmptyExternal(char *name);
/* @ Function: isNonEmptyExternal
   @ Description: Checks if an external symbol has any code or data attributes.
   @ Returns: True if the symbol has code or data attributes, False otherwise.
*/

Bool isNonEmptyEntry(char *name);
/* @ Function: isNonEmptyEntry
   @ Description: Checks if an entry symbol has any code or data attributes.
   @ Returns: True if the symbol has code or data attributes, False otherwise.
*/

Bool isLabelNameAlreadyTaken(char *name, ItemType type);
/* @ Function: isLabelNameAlreadyTaken
   @ Description: Checks if a label name is already used by a symbol or macro.
   @ Returns: True if the label name is already taken, False otherwise.
*/

Item *updateSymbolAddressValue(char *name, int newValue);
/* @ Function: updateSymbolAddressValue
   @ Description: Updates the address value of a symbol.
   @ Returns: Pointer to the updated symbol or NULL if not found.
*/

Item *getMacro(char *s);
/* @ Function: getMacro
   @ Description: Looks up and returns a macro by its name.
   @ Returns: Pointer to the found macro or NULL if not found.
*/

Item *addMacro(char *name, int start, int end);
/* @ Function: addMacro
   @ Description: Adds a new macro to the macro table with its start and end positions.
   @ Returns: Pointer to the newly added macro or NULL if the name is already in use.
*/

Item *updateMacro(char *name, int start, int end);
/* @ Function: updateMacro
   @ Description: Updates the start and end positions of an existing macro.
   @ Returns: Pointer to the updated macro or NULL if not found.
*/

void updateFinalSymbolTableValues();
/* @ Function: updateFinalSymbolTableValues
   @ Description: Updates the final values of all symbols in the symbol table after the first pass.
*/

void updateFinalValueOfSingleItem(Item *item);
/* @ Function: updateFinalValueOfSingleItem
   @ Description: Updates the final value of a single symbol item based on its attributes.
*/

Bool areEntriesExist();
/* @ Function: areEntriesExist
   @ Description: Checks if there are any entry symbols in the symbol table.
   @ Returns: True if entry symbols exist, False otherwise.
*/

Bool areExternalsExist();
/* @ Function: areExternalsExist
   @ Description: Checks if there are any external symbols in the symbol table.
   @ Returns: True if external symbols exist, False otherwise.
*/

void writeExternalsToFile(FILE *fp);
/* @ Function: writeExternalsToFile
   @ Description: Writes all external symbols and their positions to the specified file.
*/

void writeSingleExternal(FILE *fp, char *name, unsigned base, unsigned offset, ExtPositionData *next);
/* @ Function: writeSingleExternal
   @ Description: Writes a single external symbol and its positions to the specified file.
*/

void writeEntriesToFile(FILE *fp);
/* @ Function: writeEntriesToFile
   @ Description: Writes all entry symbols and their final addresses to the specified file.
*/

int writeSingleEntry(Item *item, FILE *fp, int count);
/* @ Function: writeSingleEntry
   @ Description: Writes a single entry symbol and its final address to the specified file.
*/

void initTables();
/* @ Function: initTables
   @ Description: Initializes the symbol and macro tables, clearing any existing data.
*/

void freeHashTable(ItemType type);
/* @ Function: freeHashTable
   @ Description: Frees all elements in the hash table (either symbol or macro table).
*/

void printMacroTable();
/* @ Function: printMacroTable
   @ Description: Prints the macro table, including macro names and their start and end positions.
*/

int printMacroItem(Item *item);
/* @ Function: printMacroItem
   @ Description: Prints a single macro's name, start, and end positions.
   @ Returns: Always returns 0.
*/

void printSymbolTable();
/* @ Function: printSymbolTable
   @ Description: Prints the symbol table, including symbol names, values, base addresses, offsets, and attributes.
*/

int printSymbolItem(Item *item);
/* @ Function: printSymbolItem
   @ Description: Prints a single symbol's details.
   @ Returns: Always returns 0.
*/
