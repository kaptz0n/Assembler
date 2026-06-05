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