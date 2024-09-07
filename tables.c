#include "data.h"

/**
 * Global variables for the symbol and macro tables, entry/external counts, and external list.
 * - symbols: Hash table storing symbols.
 * - macros: Hash table storing macros.
 * - entriesCount: Counter for the number of entry symbols.
 * - externalCount: Counter for the number of external symbols.
 * - extListHead: Head of the external list.
 */
static Item *symbols[HASHSIZE] = {0};
static Item *macros[HASHSIZE] = {0};
static unsigned entriesCount = 0;
static unsigned externalCount = 0;
static ExtListItem *extListHead = NULL;

extern unsigned getICF();
extern Bool verifyLabelNaming(char *s);

/**
 * resetExtList: Frees all elements in the external list and resets the external count.
 */
void resetExtList();

/**
 * findExtOpListItem: Finds an external list item by its name.
 *
 * Parameters:
 * - name: The name of the external list item.
 *
 * Returns:
 * - ExtListItem*: Pointer to the found external list item or NULL if not found.
 */
ExtListItem *findExtOpListItem(char *name);

/**
 * updateExtPositionData: Updates the base and offset for an external symbol's position.
 *
 * Parameters:
 * - name: The name of the external symbol.
 * - base: The base address of the symbol.
 * - offset: The offset address of the symbol.
 */
void updateExtPositionData(char *name, unsigned base, unsigned offset);

/**
 * freeTableItem: Frees a table item and its linked list of items.
 *
 * Parameters:
 * - item: Pointer to the item to free.
 */
void freeTableItem(Item *item);

/**
 * findExtOpListItem
 * ---------
 * Finds an external list item by its name.
 *
 * Parameters:
 * - name: The name of the external list item.
 *
 * Returns:
 * - ExtListItem*: The found item or NULL if it doesn't exist.
 */
ExtListItem *findExtOpListItem(char *name)
{
    extern ExtListItem *extListHead;
    ExtListItem *p = extListHead;
    while (p != NULL)
    {
        if (strcmp(name, p->name) == 0)
            return p; /* Return the found item */
        p = p->next;  /* Move to the next item in the list */
    }
    return NULL;
}

/**
 * resetExtList
 * ------
 * Frees all elements in the external list and resets the external count to 0.
 */
void resetExtList()
{
    ExtListItem *np = extListHead, *next;
    ExtPositionData *pos, *nextPos;
    externalCount = 0; /* Reset external count */

    while (np != NULL)
    {
        next = np->next;
        nextPos = np->value.next;
        while (nextPos != NULL)
        {
            pos = nextPos;
            nextPos = pos->next;
            free(pos); /* Free the position data */
        }
        free(np); /* Free the external list item */
        np = next;
    }

    extListHead = NULL; /* Reset the head of the external list */
}

/**
 * updateExtPositionData
 * -----------
 * Updates the position data (base and offset) for an external symbol.
 *
 * Parameters:
 * - name: The name of the external symbol.
 * - base: The base address of the symbol.
 * - offset: The offset address of the symbol.
 */
void updateExtPositionData(char *name, unsigned base, unsigned offset)
{
    ExtListItem *np = findExtOpListItem(name);

    if (np->value.base)
    {
        /* If base exists, create a new position data entry and update the list */
        ExtPositionData *new = (ExtPositionData *)malloc(sizeof(ExtPositionData));
        new->base = base;
        new->offset = offset;
        new->next = np->value.next;
        np->value.next = new;
    }
    else
    {
        /* If no base exists, set base and offset */
        np->value.base = base;
        np->value.offset = offset;
    }

    externalCount++; /* Increment external count */
}

/**
 * addExtListItem
 * -------
 * Adds a new item to the external list.
 *
 * Parameters:
 * - name: The name of the new external symbol.
 */
void addExtListItem(char *name)
{
    ExtListItem *next;
    next = (ExtListItem *)calloc(1, sizeof(ExtListItem)); /* Allocate memory for the new list item */
    strncpy(next->name, name, strlen(name));              /* Set the name of the new external symbol */

    if (extListHead != NULL)
    {
        /* Insert the new item at the beginning of the list */
        next->next = extListHead->next;
        extListHead->next = next;
    }
    else
    {
        /* Initialize the list with the new item */
        next->next = NULL;
        extListHead = next;
    }
}

/**
 * hash
 * --
 * Generates a hash value for a string. This is used for hashing symbols and macros.
 *
 * Parameters:
 * - s: The string to hash.
 *
 * Returns:
 * - unsigned: The hash value of the string.
 */
unsigned hash(char *s)
{
    unsigned hashval = 1;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval; /* Hashing algorithm using ASCII values */
    return hashval % HASHSIZE;       /* Return the hash index */
}

/**
 * lookup
 * ---
 * Looks up a symbol or macro by its name.
 *
 * Parameters:
 * - s: The name of the symbol or macro.
 * - type: The type (Symbol or Macro) to look up.
 *
 * Returns:
 * - Item*: Pointer to the found item or NULL if not found.
 */
Item *lookup(char *s, ItemType type)
{
    Item *np;
    int i = hash(s); /* Get the hash index */
    for (np = (type == Symbol ? symbols[i] : macros[i]); np != NULL; np = np->next)
        if (!strcmp(s, np->name))
            return np; /* Return the found item if names match */

    return NULL;
}

/**
 * install
 * ----
 * Adds a new symbol or macro to the symbol/macro table.
 *
 * Parameters:
 * - name: The name of the symbol or macro.
 * - type: The type (Symbol or Macro) to add.
 *
 * Returns:
 * - Item*: Pointer to the newly added item or NULL if memory allocation fails.
 */
Item *install(char *name, ItemType type)
{
    unsigned hashval;
    Item *np;
    np = (Item *)malloc(sizeof(Item)); /* Allocate memory for the new item */

    if (np == NULL)
    {
        reportError(memoryAllocationFailure); /* Report memory allocation failure */
        return NULL;
    }
    else
    {
        /* Initialize the new item with the given name and default attributes */
        np->name = cloneString(name);
        if (type == Symbol)
        {
            np->val.s.attrs.code = 0;
            np->val.s.attrs.entry = 0;
            np->val.s.attrs.external = 0;
            np->val.s.attrs.data = 0;
            np->val.s.base = 0;
            np->val.s.value = 0;
            np->val.s.offset = 0;
        }
        else if (type == Macro)
        {
            np->val.m.start = -1;
            np->val.m.end = -1;
        }

        /* Insert the new item into the appropriate hash table */
        hashval = hash(name);
        np->next = (type == Symbol ? symbols[hashval] : macros[hashval]);
        if (type == Symbol)
            symbols[hashval] = np;
        else
            macros[hashval] = np;
    }

    return np;
}

/**
 * addSymbol
 * -----
 * Adds a new symbol to the symbol table with the given attributes (code, data, entry, external).
 *
 * Parameters:
 * - name: The name of the symbol.
 * - value: The value (address) of the symbol.
 * - isCode: Whether the symbol is a code symbol.
 * - isData: Whether the symbol is a data symbol.
 * - isEntry: Whether the symbol is an entry symbol.
 * - isExternal: Whether the symbol is an external symbol.
 *
 * Returns:
 * - Bool: True if the symbol is successfully added, False otherwise.
 */
Bool addSymbol(char *name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    unsigned base;
    unsigned offset;
    Item *p;

    if (name[strlen(name) - 1] == ':')
        name[strlen(name) - 1] = '\0'; /* Remove colon from the symbol name */

    if (!verifyLabelNamingAndPrintErrors(name))
        return False; /* Return False if the label naming is not valid */

    p = lookup(name, Symbol); /* Look up the symbol in the table */
    if (p != NULL)
        return updateSymbol(p, value, isCode, isData, isEntry, isExternal); /* Update symbol if it already exists */
    else
    {
        /* Install a new symbol */
        p = install(name, Symbol);
        offset = value % 16;
        base = value - offset;
        p->val.s.value = value;
        p->val.s.base = base;
        p->val.s.offset = offset;
        p->val.s.attrs.code = isCode ? 1 : 0;
        p->val.s.attrs.entry = isEntry ? 1 : 0;
        p->val.s.attrs.external = isExternal ? 1 : 0;
        p->val.s.attrs.data = isData ? 1 : 0;
    }

    return True;
}

/**
 * updateSymbol
 * ------
 * Updates an existing symbol in the symbol table.
 *
 * Parameters:
 * - p: The symbol to update.
 * - value: The new value (address) for the symbol.
 * - isCode: Whether the symbol is a code symbol.
 * - isData: Whether the symbol is a data symbol.
 * - isEntry: Whether the symbol is an entry symbol.
 * - isExternal: Whether the symbol is an external symbol.
 *
 * Returns:
 * - Bool: True if the symbol is updated successfully, False otherwise.
 */
Bool updateSymbol(Item *p, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    if ((p->val.s.attrs.external) && (value || isData || isEntry || isCode))
        return reportError(illegalOverrideOfExternalSymbol); /* Report error if trying to override an external symbol */

    else if ((p->val.s.attrs.code || p->val.s.attrs.data || p->val.s.attrs.entry) && isExternal)
        return reportError(illegalOverrideOfLocalSymbolWithExternalSymbol); /* Report error if overriding local symbol */

    else
    {
        if ((isData && isCode) || (isCode && p->val.s.attrs.data) || (isData && p->val.s.attrs.code))
            return reportError(illegalSymbolNameAlreadyInUse); /* Error if conflicting symbol attributes */

        if (value)
        {
            unsigned base = 0;
            unsigned offset = 0;
            offset = value % 16;
            base = value - offset;
            p->val.s.value = value;
            p->val.s.base = base;
            p->val.s.offset = offset;
        }

        if (isEntry)
            p->val.s.attrs.entry = 1;
        if (isCode)
            p->val.s.attrs.code = 1;
        if (isData)
            p->val.s.attrs.data = 1;
    }

    return True;
}

/**
 * getSymbol
 * -----
 * Looks up and returns a symbol by its name.
 *
 * Parameters:
 * - name: The name of the symbol to look up.
 *
 * Returns:
 * - Item*: Pointer to the found symbol or NULL if not found.
 */
Item *getSymbol(char *name)
{
    return lookup(name, Symbol);
}

/**
 * getSymbolBaseAddress
 * ----------
 * Returns the base address of a symbol.
 *
 * Parameters:
 * - name: The name of the symbol.
 *
 * Returns:
 * - int: The base address of the symbol or -1 if not found.
 */
int getSymbolBaseAddress(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return -1; /* Return -1 if the symbol is not found */

    return p->val.s.base; /* Return the base address */
}

/**
 * getSymbolAddress
 * --------
 * Returns the value (address) of a symbol.
 *
 * Parameters:
 * - name: The name of the symbol.
 *
 * Returns:
 * - int: The value (address) of the symbol or -1 if not found.
 */
int getSymbolAddress(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return -1; /* Return -1 if the symbol is not found */

    return p->val.s.value; /* Return the symbol's value */
}

/**
 * getSymbolOffset
 * --------
 * Returns the offset of a symbol.
 *
 * Parameters:
 * - name: The name of the symbol.
 *
 * Returns:
 * - int: The offset of the symbol or -1 if not found.
 */
int getSymbolOffset(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return -1; /* Return -1 if the symbol is not found */

    return p->val.s.offset; /* Return the offset */
}

/**
 * isSymbolExist
 * -------
 * Checks if a symbol exists in the symbol table.
 *
 * Parameters:
 * - name: The name of the symbol.
 *
 * Returns:
 * - Bool: True if the symbol exists, False otherwise.
 */
Bool isSymbolExist(char *name)
{
    return lookup(name, Symbol) != NULL ? True : False;
}

/**
 * isExternal
 * -----
 * Checks if a symbol is marked as external.
 *
 * Parameters:
 * - name: The name of the symbol.
 *
 * Returns:
 * - Bool: True if the symbol is external, False otherwise.
 */
Bool isExternal(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False; /* Return False if the symbol is not found */
    return p->val.s.attrs.external;
}

/**
 * isEntry
 * ----
 * Checks if a symbol is marked as an entry.
 *
 * Parameters:
 * - name: The name of the symbol.
 *
 * Returns:
 * - Bool: True if the symbol is an entry, False otherwise.
 */
Bool isEntry(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False; /* Return False if the symbol is not found */

    return p->val.s.attrs.entry == 1 ? True : False;
}

/**
 * isNonEmptyExternal
 * ---------
 * Checks if an external symbol has any code or data attributes.
 *
 * Parameters:
 * - name: The name of the symbol.
 *
 * Returns:
 * - Bool: True if the symbol has code or data attributes, False otherwise.
 */
Bool isNonEmptyExternal(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;

    return (p->val.s.attrs.code || p->val.s.attrs.data) ? True : False;
}

/**
 * isNonEmptyEntry
 * --------
 * Checks if an entry symbol has any code or data attributes.
 *
 * Parameters:
 * - name: The name of the symbol.
 *
 * Returns:
 * - Bool: True if the symbol has code or data attributes, False otherwise.
 */
Bool isNonEmptyEntry(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;
    return (p->val.s.attrs.code || p->val.s.attrs.data) ? True : False;
}

/**
 * isLabelNameAlreadyTaken
 * ------------
 * Checks if a label name is already used by a symbol or macro.
 *
 * Parameters:
 * - name: The label name to check.
 * - type: The type (Symbol or Macro) to check for.
 *
 * Returns:
 * - Bool: True if the label name is already taken, False otherwise.
 */
Bool isLabelNameAlreadyTaken(char *name, ItemType type)
{
    Item *p = lookup(name, type);

    if (name[strlen(name) - 1] == ':')
        name[strlen(name) - 1] = '\0'; /* Remove colon from the label name */

    if (p != NULL)
    {
        if (type == Symbol)
        {
            if (p->val.s.attrs.data || p->val.s.attrs.code)
                return True;
            if (p->val.s.attrs.entry)
                return (!p->val.s.attrs.data && !p->val.s.attrs.code && !p->val.s.attrs.external) ? False : True;
            if (p->val.s.attrs.external)
                return (!p->val.s.attrs.data && !p->val.s.attrs.code && !p->val.s.attrs.entry) ? False : True;
        }

        else if (type == Macro)
            return p->val.m.start != -1 ? False : True;
    }

    return False;
}

/**
 * updateSymbolAddressValue
 * ------------
 * Updates the address value of a symbol.
 *
 * Parameters:
 * - name: The name of the symbol.
 * - newValue: The new address value for the symbol.
 *
 * Returns:
 * - Item*: Pointer to the updated symbol or NULL if not found.
 */
Item *updateSymbolAddressValue(char *name, int newValue)
{
    Item *p = getSymbol(name);
    unsigned base;
    unsigned offset;

    if (p != NULL)
    {
        offset = newValue % 16;
        base = newValue - offset;
        p->val.s.offset = offset;
        p->val.s.base = base;
        p->val.s.value = newValue;
    }
    else
        reportError(symbolDoesNotExist); /* Report error if symbol does not exist */

    return p;
}

/**
 * getMacro
 * ----
 * Looks up and returns a macro by its name.
 *
 * Parameters:
 * - s: The name of the macro.
 *
 * Returns:
 * - Item*: Pointer to the found macro or NULL if not found.
 */
Item *getMacro(char *s)
{
    return lookup(s, Macro);
}

/**
 * addMacro
 * ----
 * Adds a new macro to the macro table with its start and end positions.
 *
 * Parameters:
 * - name: The name of the macro.
 * - start: The start position of the macro.
 * - end: The end position of the macro.
 *
 * Returns:
 * - Item*: Pointer to the newly added macro or NULL if the name is already in use.
 */
Item *addMacro(char *name, int start, int end)
{
    Item *macro = lookup(name, Macro);

    if (macro != NULL)
    {
        reportError(illegalMacroNameAlreadyInUse); /* Report error if the macro name is already in use */
        return NULL;
    }
    else
    {
        macro = install(name, Macro);

        if (start != -1)
            macro->val.m.start = start;
        if (end != -1)
            macro->val.m.end = end;
    }

    return macro;
}

/**
 * updateMacro
 * ------
 * Updates the start and end positions of an existing macro.
 *
 * Parameters:
 * - name: The name of the macro.
 * - start: The new start position of the macro.
 * - end: The new end position of the macro.
 *
 * Returns:
 * - Item*: Pointer to the updated macro or NULL if not found.
 */
Item *updateMacro(char *name, int start, int end)
{
    Item *macro = getMacro(name);
    if (!macro)
        return NULL;
    if (start != -1)
        macro->val.m.start = start;
    if (end != -1)
        macro->val.m.end = end;

    return macro;
}

/**
 * updateFinalSymbolTableValues
 * --------------
 * Updates the final values of all symbols in the symbol table after the first pass.
 * This function adjusts the values of data symbols based on the final ICF value.
 */
void updateFinalSymbolTableValues()
{
    int i = 0;
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL)
            updateFinalValueOfSingleItem(symbols[i]);
        i++;
    }
}

/**
 * updateFinalValueOfSingleItem
 * --------------
 * Updates the final value of a single symbol item based on its attributes.
 * Entry symbols are counted, and external symbols are added to the external list.
 * Data symbols have their values updated based on the ICF value.
 *
 * Parameters:
 * - item: The symbol to update.
 */
void updateFinalValueOfSingleItem(Item *item)
{
    if (item->val.s.attrs.entry)
        entriesCount++; /* Count the entry symbols */
    if (item->val.s.attrs.external)
        addExtListItem(item->name); /* Add external symbols to the external list */

    if (item->val.s.attrs.data)
    {
        unsigned base = 0, offset = 0, newValue = item->val.s.value + getICF();
        offset = newValue % 16;
        base = newValue - offset;
        item->val.s.offset = offset;
        item->val.s.base = base;
        item->val.s.value = newValue;
    }

    if (item->next != NULL)
        updateFinalValueOfSingleItem(item->next); /* Recursively update the next symbol */
}

/**
 * areEntriesExist
 * --------
 * Checks if there are any entry symbols in the symbol table.
 *
 * Returns:
 * - Bool: True if entry symbols exist, False otherwise.
 */
Bool areEntriesExist()
{
    return entriesCount > 0 ? True : False;
}

/**
 * areExternalsExist
 * ---------
 * Checks if there are any external symbols in the symbol table.
 *
 * Returns:
 * - Bool: True if external symbols exist, False otherwise.
 */
Bool areExternalsExist()
{
    return externalCount > 0 ? True : False;
}

/**
 * writeExternalsToFile
 * ----------
 * Writes all external symbols and their positions to the specified file.
 *
 * Parameters:
 * - fp: The file pointer to write to.
 */
void writeExternalsToFile(FILE *fp)
{
    ExtListItem *p = extListHead;
    while (p != NULL)
    {
        if (p->value.base)
            writeSingleExternal(fp, p->name, p->value.base, p->value.offset, p->value.next); /* Write each external symbol */
        p = p->next;
    }
}

/**
 * writeSingleExternal
 * ----------
 * Writes a single external symbol and its positions to the specified file.
 * If the symbol has multiple positions, this function recursively writes them.
 *
 * Parameters:
 * - fp: The file pointer to write to.
 * - name: The name of the external symbol.
 * - base: The base address of the symbol.
 * - offset: The offset of the symbol.
 * - next: The next position data to write (if any).
 */
void writeSingleExternal(FILE *fp, char *name, unsigned base, unsigned offset, ExtPositionData *next)
{
    fprintf(fp, "%s %04u\n", name, base); /* Write the external symbol and its base address */

    if (next != NULL)
        writeSingleExternal(fp, name, next->base, next->offset, next->next); /* Recursively write next position */
}

/**
 * writeEntriesToFile
 * ---------
 * Writes all entry symbols and their final addresses to the specified file.
 *
 * Parameters:
 * - fp: The file pointer to write to.
 */
void writeEntriesToFile(FILE *fp)
{
    int i = 0;
    int totalCount = 0;
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL && totalCount <= entriesCount)
            totalCount += writeSingleEntry(symbols[i], fp, 0); /* Write entry symbols */
        i++;
    }
}

/**
 * writeSingleEntry
 * --------
 * Writes a single entry symbol and its final address to the specified file.
 * If the symbol has a linked list of items, this function recursively writes them.
 *
 * Parameters:
 * - item: The symbol to write.
 * - fp: The file pointer to write to.
 * - count: The current count of entries written.
 *
 * Returns:
 * - int: The updated count of entries written.
 */
int writeSingleEntry(Item *item, FILE *fp, int count)
{
    if (item->val.s.attrs.entry)
    {
        fprintf(fp, "%s %04d\n", item->name, item->val.s.base + item->val.s.offset); /* Write the entry symbol */
        count++;
    }
    if (item->next != NULL)
        writeSingleEntry(item->next, fp, count); /* Recursively write next entry */

    return count;
}

/**
 * initTables
 * -----
 * Initializes the symbol and macro tables, clearing any existing data.
 * Also resets the external and entry symbol counters.
 */
void initTables()
{
    extern unsigned externalCount, entriesCount;
    int i = 0;
    if (extListHead != NULL)
        resetExtList(); /* Reset the external list */

    externalCount = entriesCount = 0;
    while (i < HASHSIZE)
    {
        symbols[i] = NULL;
        macros[i] = NULL; /* Clear the symbol and macro tables */
        i++;
    }
}

/**
 * freeHashTable
 * -------
 * Frees all elements in the hash table (either symbol or macro table) based on the specified type.
 *
 * Parameters:
 * - type: The type of items to free (Symbol or Macro).
 */
void freeHashTable(ItemType type)
{
    int i = 0;
    while (i < HASHSIZE)
    {
        if (type == Symbol)
        {
            if (symbols[i] != NULL)
                freeTableItem(symbols[i]); /* Free symbol table items */
        }
        else
        {
            if (macros[i] != NULL)
                freeTableItem(macros[i]); /* Free macro table items */
        }
        i++;
    }
}

/**
 * freeTableItem
 * -------
 * Recursively frees a linked list of table items.
 *
 * Parameters:
 * - item: Pointer to the item to free.
 */
void freeTableItem(Item *item)
{
    if (item->next != NULL)
        freeTableItem(item->next); /* Recursively free the next item */
    free(item);
    return;
}

/**
 * printMacroTable
 * --------
 * Prints the macro table, including macro names and their start and end positions.
 */
void printMacroTable()
{
    int i = 0;
    printf("\n\t ~ MACRO TABLE ~ \n");
    printf("\tname\tstart\tend");
    while (i < HASHSIZE)
    {
        if (macros[i] != NULL)
            printMacroItem(macros[i]); /* Print each macro */
        i++;
    }
    printf("\n\n");
}

/**
 * printMacroItem
 * -------
 * Prints a single macro's name, start, and end positions.
 * If the macro has a linked list of items, this function recursively prints them.
 *
 * Parameters:
 * - item: The macro to print.
 *
 * Returns:
 * - int: Always returns 0.
 */
int printMacroItem(Item *item)
{
    printf("\n\t%s\t %5d\t%6d", item->name, item->val.m.start, item->val.m.end); /* Print macro details */
    if (item->next != NULL)
        printMacroItem(item->next); /* Recursively print next macro */
    return 0;
}

/**
 * printSymbolTable
 * --------
 * Prints the symbol table, including symbol names, values, base addresses, offsets, and attributes.
 */
void printSymbolTable()
{
    int i = 0;

    printf("\n\t\t ~ SYMBOL TABLE ~ \n");
    printf("name\tvalue\tbase\toffset\tattributes");

    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL)
            printSymbolItem(symbols[i]); /* Print each symbol */
        i++;
    }
    printf("\n\n");
}

/**
 * printSymbolItem
 * --------
 * Prints a single symbol's details, including its name, value, base address, offset, and attributes.
 * If the symbol has a linked list of items, this function recursively prints them.
 *
 * Parameters:
 * - item: The symbol to print.
 *
 * Returns:
 * - int: Always returns 0.
 */
int printSymbolItem(Item *item)
{
    printf("\n%s\t%u\t%u\t%u\t", item->name, item->val.s.value, item->val.s.base, item->val.s.offset); /* Print symbol details */
    if (!item->val.s.attrs.code && !item->val.s.attrs.data && !item->val.s.attrs.entry && !item->val.s.attrs.external)
        printf("   ");

    else
    {
        if ((item->val.s.attrs.code || item->val.s.attrs.data) && (item->val.s.attrs.entry || item->val.s.attrs.external))
        {
            if (item->val.s.attrs.code)
                printf("code,");
            else
                printf("data,");

            if (item->val.s.attrs.entry)
                printf("entry");
            else
                printf("external");
        }
        else
        {
            if (item->val.s.attrs.code)
                printf("code");
            else if (item->val.s.attrs.data)
                printf("data");
            else if (item->val.s.attrs.entry)
                printf("entry");
            else
                printf("external");
        }
    }

    if (item->next != NULL)
        printSymbolItem(item->next); /* Recursively print next symbol */
    return 0;
}
