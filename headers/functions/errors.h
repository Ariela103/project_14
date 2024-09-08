
/*

--------------------------- errors.c/h ---------------------------------------------------------------------
This file is used for handling different errors our assembler might encounter during all the stages of the
assembler. the errors and printed to the stderr and to a %sourceFileName%.am.errors.log file that will be
 created for each source file within source's file directory.
-------------------------------------------------------------------------------------------------------------

*/

/**
 * @brief Finds or creates a log file entry for the given file name.
 *
 * @param fileName The name of the file for which log files are being handled.
 * @return LogFiles* Pointer to the LogFiles entry for the given file name.
 */
LogFiles *getLogFiles(char *fileName);

/**
 * @brief Opens an error log file for the specified LogFiles entry.
 *
 * @param logFile A pointer to the LogFiles structure that contains file information.
 */
void openErrorLogFile(LogFiles *logFile);

/**
 * @brief Opens a warning log file for the specified LogFiles entry.
 *
 * @param logFile A pointer to the LogFiles structure that contains file information.
 */
void openWarningLogFile(LogFiles *logFile);

/**
 * @brief Handles a file creation failure and logs the error.
 *
 * @param fileName The name of the file that failed to be created.
 */
void fileCreationFailure(char *fileName);

/**
 * @brief Handles a file opening failure and logs the error.
 *
 * @param fileName The name of the file that failed to open.
 */
void fileOpeningFailure(char *fileName);

/**
 * @brief Logs a warning to the warning log file for the specified file.
 *
 * @param err The warning to log.
 * @param fileName The name of the file for which the warning is being logged.
 */
void reportWarningIntoFile(Warning err, char *fileName);

/**
 * @brief Logs an error to the error log file for the specified file.
 *
 * @param err The error to log.
 * @param fileName The name of the file for which the error is being logged.
 */
void reportErrorIntoFile(Error err, char *fileName);

/**
 * @brief Reports a warning message to the console and log file.
 *
 * @param err The warning to log.
 * @return Bool Returns True if the warning was successfully reported.
 */
Bool reportWarning(Warning err);

/**
 * @brief Reports an error message to the console and log file.
 *
 * @param err The error to log.
 * @return Bool Returns False after reporting the error.
 */
Bool reportError(Error err);

/**
 * @brief Closes all open log files and resets the log file system.
 */
void closeOpenLogFiles(void);
