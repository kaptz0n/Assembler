#ifndef EXPORTER_H
#define EXPORTER_H
#include "symbols.h"

/* 
Changes the extension of a string representation of a filename 
szFilename - The filename to modify.
szNewExt - The new extension to apply.
*/
void change_extension(char *szFilename, const char *szNewExt);


/*
Creates the .ob, .ent, and .ext files based on the results of the assembler process.
szOriginalFilename - The original .as filename.
finalIC - The final IC value after the first pass.
finalDC - The final DC value after the first pass.
pCodeImage - The array containing the code image.
pDataImage - The array containing the data image.
pSymbolTable - The symbol table generated during the first pass.
pExtList - The list of external symbols generated during the second pass.
*/
void export_files(const char *szOriginalFilename, int finalIC, int finalDC, unsigned int *pCodeImage, unsigned char *pDataImage, SymbolNode *pSymbolTable, ExtNode *pExtList);


#endif