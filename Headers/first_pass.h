#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "symbols.h"

/*
Runs the First Pass on a pre-processed .am file.
pAmFile - Pointer to the open .am file.
pSymbolTable - Pointer to the head of the symbol table linked list.
pDataImage - Array where data bytes (.db, .dh, .dw, .asciz) are stored.
pCodeImage - Array where opcodes are stored.
pFinalDC - Pointer to return the final Data Counter size.
pFinalIC - Pointer to return the final Instruction Counter size.
Output:
Returns 0 on success, 1 on failure.
*/
int first_pass(FILE *pAmFile, SymbolNode **pSymbolTable, unsigned char *pDataImage, unsigned int *pCodeImage, int *pFinalDC, int *pFinalIC);

#endif