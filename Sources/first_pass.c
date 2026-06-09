#include <stdio.h>
#include <string.h>
#include "../Headers/first_pass.h"
#include "../Headers/parser.h"
#include "../Headers/analyzers.h"
#include "../Headers/errors.h"
#include "../Headers/globals.h"


int first_pass(FILE *pAmFile, SymbolNode **pSymbolTable, unsigned char *pDataImage, unsigned int *pCodeImage, int *pFinalDC, int *pFinalIC)
{
    char szLine[MAX_LINE_LENGTH];
    ParsedLine parsedLine;
    int nLineNumber = 0;
    int bErrorFound = 0;

    /* Starting addresses */
    int IC = 100;
    int DC = 0;

    while (fgets(szLine, MAX_LINE_LENGTH, pAmFile) != NULL)
    {
        nLineNumber++;

        /* Parse the line into parts */
        if (parse_line(szLine, &parsedLine, nLineNumber) == 1)
        {
            bErrorFound = 1;
            continue;
        }

        /* If the line was empty or just a comment (szCommand for comment is empty), skip to the next */
        if (strlen(parsedLine.szCommand) == 0)
        {
            continue;
        }

        /* Check for .extern or .entry */
        if (strcmp(parsedLine.szCommand, ".extern") == 0)
        {
            if (!is_valid_label(parsedLine.szOperands))
            {
                print_error(ERR_INVALID_LABEL, nLineNumber);
                bErrorFound = 1;
            }
            else 
            {
                /* Check if the label exists as a non extern label */
                SymbolNode *pExisting = find_symbol(*pSymbolTable, parsedLine.szOperands);
                
                if (pExisting != NULL)
                {
                    if (pExisting->eType != SYMBOL_EXTERN)
                    {
                        print_error(ERR_LABEL_ALREADY_DEFINED, nLineNumber);
                        bErrorFound = 1;
                    }
                }
                else
                {
                    add_symbol(pSymbolTable, parsedLine.szOperands, 0, SYMBOL_EXTERN);
                }
            }
            continue; 
        }
        else if (strcmp(parsedLine.szCommand, ".entry") == 0)
        {
            continue;
        }

        /* Handle Data Directives  */
        if (parsedLine.szCommand[0] == '.')
        {
            /* Checks if it's a label*/
            if (strlen(parsedLine.szLabel) > 0)
            {
                /* Checks for duplicate labels */
                if (find_symbol(*pSymbolTable, parsedLine.szLabel) != NULL)
                {
                    print_error(ERR_LABEL_ALREADY_DEFINED, nLineNumber);
                    bErrorFound = 1;
                }
                else
                {
                    add_symbol(pSymbolTable, parsedLine.szLabel, DC, SYMBOL_DATA);
                }
            }

            
            if (process_directive(&parsedLine, pDataImage, &DC, nLineNumber))
            {
                bErrorFound = 1;
            }
            continue; 
        }

        /*
        Its an instruction. 
        Checks if there is a label 
        */
       
        if (strlen(parsedLine.szLabel) > 0)
        {
            if (find_symbol(*pSymbolTable, parsedLine.szLabel) != NULL)
            {
                print_error(ERR_LABEL_ALREADY_DEFINED, nLineNumber);
                bErrorFound = 1;
            }
            else
            {
                add_symbol(pSymbolTable, parsedLine.szLabel, IC, SYMBOL_CODE);
            }
        }

        /* Send to the Instruction Analyzer to calculate the 32-bit opcode */
        if (process_instruction(&parsedLine, pCodeImage, &IC, nLineNumber))
        {
            bErrorFound = 1;
        }
    }

    /* Update all DATA symbol addresses by adding the final IC */
    update_data_symbols(*pSymbolTable, IC);

    /* Export the final counter sizes back to the main program */
    *pFinalDC = DC;
    *pFinalIC = IC;

    return bErrorFound;
}