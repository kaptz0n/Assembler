#ifndef ANALYZERS_H
#define ANALYZERS_H

#include "parser.h"

/*
Processes a data directive (.db, .dh, .dw, .asciz).
Parses the operands, inserts them into the Data Image, and updates the DC.
pLine - The parsed line with the directive and operands.
pDataImage - The array of the Data Image.
pDC - Pointer to the current DC, which will be updated as data is added.
nLineNumber - The line number in the source file.
Output:
Returns 0 on success, 1 on failure.
 */
int process_directive(ParsedLine *pLine, unsigned char *pDataImage, int *pDC, int nLineNumber);

/*
Processes a code instruction (R, I, or J type).
Validates operands, calculates the 32-bit machine word, and updates the IC.
pLine - The parsed line containing the command and operands.
pCodeImage - The array of 32-bit integers holding the machine code.
pIC - Pointer to the Instruction Counter.
nLineNumber - The current line number for error reporting.
Output:
Returns 0 on success, 1 on failure.
*/
int process_instruction(ParsedLine *pLine, unsigned int *pCodeImage, int *pIC, int nLineNumber);
#endif