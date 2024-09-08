
/*
-----------------------------------------------------------------------------------------
--------------------------- main.c General Overview: ---------------------------
-----------------------------------------------------------------------------------------
This file is the main entry point for the assembler. It processes command-line arguments,
opens and processes each provided source file, and coordinates the different stages of
the assembler's execution. The stages include:
- Macro expansion in the source file.
- The first pass, which checks for syntax errors and calculates memory requirements.
- The second pass, which finalizes memory addresses, builds the memory image, and
  updates the symbol table.
- Exporting the output files, such as the object (.ob), entries (.ent), and externals (.ext) files.

If any errors are encountered during the process, they are logged, and file export may be skipped.
-----------------------------------------------------------------------------------------
*/

/**
 * @brief The main entry point of the assembler.
 *
 * This function handles command-line arguments, passing them to the assembler for processing.
 * It calls the `handleSourceFiles` function to process each source file provided via the command line.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments (file names).
 * @return int Returns 0 upon successful execution.
 */
int main(int argc, char *argv[]);

/**
 * @brief Processes all source files passed via the command line.
 *
 * This function iterates through each file provided by the user and processes them individually.
 * If no files are provided, the program exits with an error message.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments (file names).
 * @return int Returns 0 upon successful processing of all files.
 */
int handleSourceFiles(int argc, char *argv[]);

/**
 * @brief Handles the processing of a single source file.
 *
 * This function opens the source file, creates the expanded macro file, and performs the first and second
 * passes of the assembler. It calculates memory requirements, updates the symbol table, and generates
 * output files (.ob, .ent, .ext) if no errors are encountered during the assembly process.
 *
 * @param arg The name of the source file to process.
 */
extern void handleSingleFile(char *arg);
