#ifndef ERRORS_H
#define ERRORS_H

/* All possible error codes for the assembler */
typedef enum
{
    ERR_NONE = 0,
    ERR_LINE_TOO_LONG,
    ERR_INVALID_MACRO_NAME,
    ERR_MACRO_ALREADY_DEFINED,
    ERR_EXTRA_TEXT,       
    ERR_MISSING_MACRO_NAME,
    ERR_INVALID_LABEL,
    ERR_STRING_MISSING_QUOTES,
    ERR_MISSING_DATA,
    ERR_INVALID_NUMBER_FORMAT,
    ERR_UNKNOWN_DIRECTIVE,
    ERR_LABEL_ALREADY_DEFINED,
    ERR_UNKNOWN_COMMAND,
    ERR_WRONG_OPERAND_COUNT,
    ERR_INVALID_REGISTER,
    ERR_LABEL_NOT_FOUND,
    /* Add errors before this entry */
    ERR_COUNT 
} ErrorCode;


/*
Prints the error message using the error code
eCode - The error code for which to print a message.
nLineNumber - The line number where the error occurred.
 */
void print_error(ErrorCode eCode, int nLineNumber);


#endif