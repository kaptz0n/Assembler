
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <stdio.h>
#include "globals.h"

/* Structure for a generic node in a linked list */
typedef struct LineNode
{
    char szLine[MAX_LINE_LENGTH];
    struct LineNode *pNext;
} LineNode;

/* Structure for a macro node in the macro table */
typedef struct MacroNode
{
    char szName[MAX_MACRO_NAME_LENGTH];
    LineNode *pLines;
    struct MacroNode *pNext;
} MacroNode;


/*
Adds a new macro definition to the head of the macro table.
ppHead - Pointer to the head pointer of the macro list.
szName - String name of the macro being defined.
Output:
Returns pointer to the new MacroNode, NULL on failure.
 */
MacroNode* add_macro(MacroNode **ppHead, const char *szName);


/*
Appends a line of code to the end of an existing macro.
pMacro - Pointer to the macro receiving the line.
szLine - String containing the line of code.
Output:
Returns 0 on success, nonzero on failure.
 */
int add_line_to_macro(MacroNode *pMacro, const char *szLine);


/*
Frees all allocated memory for the macro table.
pHead - Pointer to the head of the macro list.
 */
void free_macro_table(MacroNode *pHead);


/*
Searches for a macro by name in the macro table.
pHead - Pointer to the head of the macro list.
szName - The string name of the macro to find.
Output:
Returns a pointer to the MacroNode if found, NULL on failure.
 */
MacroNode* find_macro(MacroNode *pHead, const char *szName);


/*
Checks if a line is contains only whitespace characters.
szLine - The string line to check.
Output:
Returns 1 if the line has whitespaces only, otherwise 0.
*/
int is_empty_line(const char *szLine);


/*
Checks if a macro name is valid (not a reserved opcode or register).
szName - The macro name to validate.
Output:
Returns 1 if valid, otherwise 0.
 */
int is_macro_name_valid(const char *szName);


/*
Processes the input file, expands macros, and writes to the output file.
pInputFile - Pointer to the open .as source file.
pOutputFile - Pointer to the open .am destination file.
Output:
Returns 0 on success, nonzero on failure.
 */
int process_macros(FILE *pInputFile, FILE *pOutputFile);


#endif