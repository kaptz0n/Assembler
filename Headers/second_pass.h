#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include <stdio.h>
#include "symbols.h"
#include "globals.h"

/*
Performs the second pass of the assembler, resolving symbol addresses and filling the code image.
pFile - The file pointer to the .am file to read from.
pSymbolTable - The symbol table built during the first pass, used to resolve addresses.
pCodeImage - The code image array to update with resolved addresses.
pExtList - Pointer to the head of the extern usage list, which will be created during this pass.
Output:
Returns 0 on success, nonzero if errors were found.
*/
int second_pass(FILE *pFile, SymbolNode *pSymbolTable, unsigned int *pCodeImage, ExtNode **pExtList);


#endif