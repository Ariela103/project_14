
/*
##########################################################################################################
##########################################################################################################
#################################### errors.c General Overview: ##########################################
##########################################################################################################
##########################################################################################################
This file is used for handling different errors our assembler might encounter during all the stages of the
assembler. the errors and printed to the stderr and to a errors.log file that will be created in the root
directory in which the terminal is currently on.
##########################################################################################################
##########################################################################################################
##########################################################################################################
*/

Bool reportError(Error err);
void reportErrorIntoFile(Error err, char *fileName);
Bool reportWarning(Warning err);
void reportWarningIntoFile(Warning err, char *fileName);
void fileOpeningFailure(char *fileName);
void fileCreationFailure(char *fileName);
void closeOpenLogFiles();
