
/*
   -----------------------------------------------------------------------------------------
   ---------------------------  General Overview: ---------------------------
   -----------------------------------------------------------------------------------------
   In this module, we handle the first run of parsing and analyzing assembly code. We verify the
   syntax and legality of instructions, operations, operands, labels, and symbols. If errors are encountered,
   we report them in the error log and continue to the next line. During this process, we calculate the memory size
   required for both data and instruction images.

   The main goal of the first run is to gather information on memory allocation needs. Once the first run completes
   successfully without errors, the system enters the second run (indicated by the `globalState` change).

   -----------------------------------------------------------------------------------------
   -----------------------------------------------------------------------------------------
 */

/**
 * @brief Handles the parsing and validation of an operation in an assembly code line.
 *
 * This function processes the operation by splitting the operands, validating their correctness,
 * and checking whether the number of operands and addressing methods are legal for the given operation.
 *
 * @param operationName The name of the operation to handle.
 * @param args The operands/arguments for the operation.
 * @return Bool Returns True if the operation and its operands are valid, False otherwise.
 */
Bool handleOperation(char *operationName, char *args);

/**
 * @brief Parses the source and destination operands for an operation and validates them.
 *
 * This function checks if the provided operands (source and destination) are valid based on the
 * allowed addressing methods for the given operation. If errors are encountered, it logs the errors
 * and continues parsing the line to find and report all issues.
 *
 * @param src The source operand.
 * @param des The destination operand.
 * @param op The operation structure containing the allowed addressing methods.
 * @param active An array that tracks the addressing methods used by each operand.
 * @return Bool Returns True if the operands are valid, False otherwise.
 */
Bool parseOperands(char *src, char *des, const Operation *op, AddrMethodsOptions active[2]);

/**
 * @brief Validates if an operand matches the allowed addressing methods for the operation.
 *
 * This function checks whether the given operand uses a valid addressing method (direct, register, immediate, or indirect).
 * If the operand does not match the expected addressing method, it returns False and logs an error.
 *
 * @param allowedAddrs The allowed addressing methods for the source and destination operands.
 * @param active An array that stores the active addressing method for the current operand.
 * @param operand The operand string to validate.
 * @param type 0 for source operand, 1 for destination operand.
 * @return Bool Returns True if the operand is valid, False otherwise.
 */
Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, AddrMethodsOptions active[2], char *operand, int type);

/**
 * @brief Handles the parsing and validation of an assembly instruction or directive.
 *
 * This function checks if the first token is a valid instruction or label declaration. If the instruction
 * type is valid, it processes the arguments and checks for errors such as illegal characters or missing operands.
 * For label declarations, it verifies that the label is followed by a valid instruction or operation.
 *
 * @param type The type of the instruction (data, string, entry, or external).
 * @param firstToken The first token in the line (usually the instruction or label).
 * @param nextTokens The remaining tokens in the line (typically the operands/arguments).
 * @param line The full line of assembly code.
 * @return Bool Returns True if the instruction is valid and processed successfully, False otherwise.
 */
Bool handleInstruction(int type, char *firstToken, char *nextTokens, char *line);

/**
 * @brief Handles the parsing and validation of a label and its corresponding instruction or operation.
 *
 * This function processes a line of code that begins with a label declaration. It checks whether the next
 * token is a valid instruction or operation, and validates the rest of the line. If errors are found,
 * they are logged, and the function continues parsing to report all issues.
 *
 * @param labelName The name of the label being processed.
 * @param nextToken The next token after the label (typically the instruction or operation).
 * @param line The full line of assembly code.
 * @return Bool Returns True if the label and corresponding instruction/operation are valid, False otherwise.
 */
Bool handleLabel(char *labelName, char *nextToken, char *line);