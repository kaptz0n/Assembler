#include <stdio.h>
#include "errors.h"


/* Error messages */
static const char *szErrorMessages[] = 
{
    "No error",
    "Line exceeds maximum permitted length (80 characters)",
    "Macro name is invalid (reserved word or illegal characters)",
    "Macro is already defined in the macro table",
    "Extra text found at the end of the command",
    "Missing macro name in macro definition",
    "Label name is invalid (reserved word, too long, or illegal characters)",
};

void print_error(ErrorCode eCode, int nLineNumber)
{
    /* Validate that the error code exists within the enum range */
    if (eCode > ERR_NONE && eCode < ERR_COUNT)
    {
        printf("Error at line %d: %s\n", nLineNumber, szErrorMessages[eCode]);
    }
    else
    {
        printf("Unknown error code (%d) encountered at line %d\n", eCode, nLineNumber);
    }
}