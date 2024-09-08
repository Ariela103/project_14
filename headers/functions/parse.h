
/*
-----------------------------------------------------------------------------------------
--------------------------- parse.c General Overview: ---------------------------
-----------------------------------------------------------------------------------------

The purpose of `parse.c` is to handle the parsing of assembly code during the two-pass
assembly process. This file plays a critical role in interpreting each line of the assembly
source code, validating its syntax, and managing the internal states of the assembler.

Key functionalities of `parse.c`:
- **First Pass Parsing:** In the first run, the code checks for syntax errors, processes
  labels and instructions, and calculates memory addresses. Errors are reported, but the
  assembler continues to check and parse the entire input file.

- **Second Pass Parsing:** During the second pass, after ensuring the validity of the code,
  the assembler finalizes memory addresses, stores them into the data and instruction memory
  images, and prepares the data for output.

- **Main Parsing Functions:**
  - **`parseLine`:** Handles parsing of individual lines, checking for comments, labels,
    instructions, and operations. This function coordinates the validation and parsing
    process for each line.
  - **`handleSingleLine`:** Responsible for processing one line of code at a time, ensuring
    that labels, instructions, and operations are handled appropriately.
  - **`parseAssemblyCode`:** Manages the overall flow of the parsing process, iterating
    through each line in the source file and invoking the necessary functions for handling
    individual lines.
  - **Validation Functions:** Functions like `countAndVerifyDataArguments` and
    `countAndVerifyStringArguments` are used to ensure that assembly instructions are
    correctly formatted.

- **Error Handling:** The parser ensures that any errors in the code (e.g., missing commas,
  invalid operands, or illegal syntax) are detected and reported without halting the
  assembly process, allowing for full error reporting on the first pass.

- **Commas and Syntax Validation:** `verifyCommaSyntax` checks the correct placement of
  commas between arguments, ensuring that arguments follow valid assembly syntax.

- **Memory Management:** The parser also increases memory counters during parsing for both
  data and instructions to allocate the appropriate space in memory for the final binary
  output.

Overall, `parse.c` is an essential component that verifies the correctness of the assembly
code, processes it line by line, and ensures the proper assembly structure is followed
before generating the final machine code.

-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

/**
 * @brief Parses and verifies the arguments in a .data instruction.
 *
 * This function checks the syntax of the .data instruction and ensures that
 * the arguments are valid numbers. If valid, it increases the data counter.
 *
 * @param line The full line containing the .data instruction and its arguments.
 * @return Bool Returns True if the arguments are valid, False otherwise.
 */
Bool countAndVerifyDataArguments(char *line);

/**
 * @brief Verifies the correct placement of commas in a line of arguments.
 *
 * This function checks if there are any extra or missing commas in the line.
 * It ensures that commas are placed correctly between arguments.
 *
 * @param line The line of arguments to check.
 * @return Bool Returns True if the comma syntax is correct, False otherwise.
 */
Bool verifyCommaSyntax(char *line);

/**
 * @brief Verifies the arguments in a .string instruction.
 *
 * This function checks the validity of the arguments in a .string instruction,
 * ensuring that the string is enclosed in quotes. If valid, it increases the data counter.
 *
 * @param line The full line containing the .string instruction and its arguments.
 * @return Bool Returns True if the arguments are valid, False otherwise.
 */
Bool countAndVerifyStringArguments(char *line);

/**
 * @brief Parses a line of assembly code, handling comments, labels, instructions, and operations.
 *
 * This function processes each line of assembly code, detecting comments, label declarations,
 * instructions (e.g., .data, .string), and operations. It validates the syntax and structure of the line.
 *
 * @param token The first token in the line (e.g., a label, instruction, or operation).
 * @param line The full line of code.
 * @return Bool Returns True if the line is valid, False otherwise.
 */
Bool parseLine(char *token, char *line);

/**
 * @brief Handles a single line of assembly code.
 *
 * This function processes and validates a single line of assembly code. After processing,
 * it increments the current line number.
 *
 * @param line The line of code to be processed.
 * @return Bool Returns True if the line is valid, False otherwise.
 */
Bool handleSingleLine(char *line);

/**
 * @brief Parses the assembly source file and processes each line of code.
 *
 * This function reads the assembly source file and processes each line, handling both
 * the first and second pass of the assembler. It validates the structure and syntax
 * of the code, preparing it for memory allocation and binary output.
 *
 * @param src The source file to be parsed.
 */
void parseAssemblyCode(FILE *src);
