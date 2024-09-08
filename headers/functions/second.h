/*
-----------------------------------------------------------------------------------------
--------------------------- second.c/h General Overview: ---------------------------
-----------------------------------------------------------------------------------------
The `second.c` file is responsible for generating the binary representation of assembly
operations and instructions during the second pass of the assembler. It writes the
machine code to memory, handling various addressing methods and operand types.

Key Functions:
- **writeOperationBinary:** Converts an assembly operation into binary machine code and writes it to memory.
- **writeDataInstruction & writeStringInstruction:** Convert data and string instructions into machine code.
- **detectOperandType:** Determines the addressing method used for each operand.
- **writeFirstWord, writeSecondAndThirdWords:** Writes the machine code words representing the operation and its operands.

This file ensures that all assembly instructions are correctly converted to machine code
before the final object file is generated.

-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

/**
 * writeOperationBinary
 * --------------------
 * Converts an assembly operation into its binary representation and writes it to memory.
 *
 * Parameters:
 * - operationName: The name of the operation (e.g., "mov", "add").
 * - args: The arguments of the operation (registers, labels, or immediate values).
 *
 * Returns:
 * - Bool: True if the operation is valid and written successfully, False otherwise.
 */
Bool writeOperationBinary(char *operationName, char *args);

/**
 * writeSecondAndThirdWords
 * ------------------------
 * Writes the second and third machine code words for an operation, based on its operands.
 *
 * Parameters:
 * - first: The first operand (if any).
 * - second: The second operand.
 * - active: Addressing method flags for each operand.
 * - op: The operation being processed.
 */
void writeSecondAndThirdWords(char *first, char *second, AddrMethodsOptions active[2], const Operation *op);

/**
 * writeFirstWord
 * --------------
 * Writes the first machine code word for an operation, which contains the opcode and addressing methods.
 *
 * Parameters:
 * - op: The operation to write.
 * - active: Addressing method flags for each operand.
 */
void writeFirstWord(const Operation *op, AddrMethodsOptions active[2]);

/**
 * writeDirectOperandWord
 * ----------------------
 * Writes the machine code word corresponding to a direct addressing operand.
 *
 * Parameters:
 * - labelName: The name of the label or symbol for direct addressing.
 */
void writeDirectOperandWord(char *labelName);

/**
 * writeImmediateOperandWord
 * -------------------------
 * Writes the machine code word for an immediate addressing operand.
 *
 * Parameters:
 * - n: The immediate value as a string.
 */
void writeImmediateOperandWord(char *n);

/**
 * detectOperandType
 * -----------------
 * Detects the addressing method of an operand and sets the corresponding flags in the active array.
 *
 * Parameters:
 * - operand: The operand to detect.
 * - active: Array storing the addressing method flags.
 * - type: Indicates whether the operand is a source (0) or target (1).
 *
 * Returns:
 * - Bool: True if the operand type is valid, False otherwise.
 */
Bool detectOperandType(char *operand, AddrMethodsOptions active[2], int type);

/**
 * writeDataInstruction
 * --------------------
 * Converts a .data instruction into its binary representation and writes it to memory.
 *
 * Parameters:
 * - token: The first data value in the .data instruction.
 *
 * Returns:
 * - Bool: Always returns True.
 */
Bool writeDataInstruction(char *token);

/**
 * writeStringInstruction
 * ----------------------
 * Converts a .string instruction into its binary representation and writes it to memory.
 *
 * Parameters:
 * - s: The string to be written.
 *
 * Returns:
 * - Bool: Always returns True.
 */
Bool writeStringInstruction(char *s);

/**
 * parseRegNumberFromOperand
 * -------------------------
 * Extracts the register number from an operand string (e.g., "r1" becomes 1).
 *
 * Parameters:
 * - s: The operand containing the register.
 *
 * Returns:
 * - int: The register number.
 */
int parseRegNumberFromOperand(char *s);