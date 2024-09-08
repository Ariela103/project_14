/*
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
--------------------------- utils.h General Overview: ---------------------------
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------

The `utils` module contains utility functions for the assembler, which include validating
and checking various elements of the assembly code such as macro definitions, register
names, label declarations, immediate parameters, and indirect parameters. The module
also handles the detection of assembly instructions, operations, and comments, and ensures
proper label naming according to the assembly language rules.

-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

/* Register names array */
extern const char *regs[REGS_SIZE];

/* Function declarations */
Bool isMacroOpening(char *s);
/* @ Function: isMacroOpening
   @ Description: Checks if the given string is the opening of a macro.
   @ Returns: True if the string is "macr", False otherwise.
*/

Bool isMacroClosing(char *s);
/* @ Function: isMacroClosing
   @ Description: Checks if the given string is the closing of a macro.
   @ Returns: True if the string is "endmacr", False otherwise.
*/

Bool isPossiblyUseOfMacro(char *s);
/* @ Function: isPossiblyUseOfMacro
   @ Description: Checks if the given string could possibly be a macro usage.
   @ Returns: True if the string could be a macro usage, False otherwise.
*/

Bool isLegalMacroName(char *s);
/* @ Function: isLegalMacroName
   @ Description: Checks if the given string is a valid macro name.
   @ Returns: True if the string is a legal macro name, False otherwise.
*/

Bool isInstruction(char *s);
/* @ Function: isInstruction
   @ Description: Checks if the given string is an instruction (.data, .string, etc.).
   @ Returns: True if the string is an instruction, False otherwise.
*/

Bool isInstructionStrict(char *s);
/* @ Function: isInstructionStrict
   @ Description: Checks if the given string is strictly an instruction (without missing spaces).
   @ Returns: True if the string is exactly an instruction, False otherwise.
*/

Bool isRegistery(char *s);
/* @ Function: isRegistery
   @ Description: Checks if the given string is a valid register name (r0-r7).
   @ Returns: True if the string is a valid register, False otherwise.
*/

Bool isValidImmediateParamter(char *s);
/* @ Function: isValidImmediateParamter
   @ Description: Checks if the given string is a valid immediate parameter (e.g., #5).
   @ Returns: True if the string is a valid immediate parameter, False otherwise.
*/

Bool isIndirectParameter(char *s);
/* @ Function: isIndirectParameter
   @ Description: Checks if the given string is a valid indirect parameter (e.g., *r0).
   @ Returns: True if the string is a valid indirect parameter, False otherwise.
*/

Bool isValidIndirectParameter(char *s);
/* @ Function: isValidIndirectParameter
   @ Description: Checks if the string is a valid indirect parameter referencing a valid register.
   @ Returns: True if the string is a valid indirect parameter, False otherwise.
*/

Bool isComment(char *s);
/* @ Function: isComment
   @ Description: Checks if the given string is a comment (starts with ';').
   @ Returns: True if the string is a comment, False otherwise.
*/

Bool isOperation(char *s);
/* @ Function: isOperation
   @ Description: Checks if the given string is a valid operation name.
   @ Returns: True if the string is a valid operation, False otherwise.
*/

Bool isLabelDeclarationStrict(char *s);
/* @ Function: isLabelDeclarationStrict
   @ Description: Checks if the given string is a strict label declaration (ends with ':').
   @ Returns: True if the string is a label declaration, False otherwise.
*/

Bool isLabelDeclaration(char *s);
/* @ Function: isLabelDeclaration
   @ Description: Checks if the given string contains a label declaration (contains ':').
   @ Returns: True if the string is a label declaration, False otherwise.
*/

int getInstructionType(char *s);
/* @ Function: getInstructionType
   @ Description: Determines the instruction type based on the given string (e.g., .data).
   @ Returns: The type of instruction or False if not an instruction.
*/

int getRegisteryNumber(char *s);
/* @ Function: getRegisteryNumber
   @ Description: Extracts the register number from the given register string (e.g., r2 -> 2).
   @ Returns: The register number.
*/

char *getInstructionNameByType(int type);
/* @ Function: getInstructionNameByType
   @ Description: Returns the instruction name corresponding to the given type.
   @ Returns: The instruction name or NULL if not found.
*/

char *getInstructionName(char *s);
/* @ Function: getInstructionName
   @ Description: Returns the instruction name based on the given string.
   @ Returns: The instruction name or 0 if not found.
*/

Bool verifyLabelNaming(char *s);
/* @ Function: verifyLabelNaming
   @ Description: Verifies that the label name is valid.
   @ Returns: True if the label name is valid, False otherwise.
*/

Bool verifyLabelNamingAndPrintErrors(char *s);
/* @ Function: verifyLabelNamingAndPrintErrors
   @ Description: Verifies that the label name is valid and prints errors if not.
   @ Returns: True if the label name is valid, False otherwise.
*/
