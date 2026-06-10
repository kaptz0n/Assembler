#include <stdio.h>
#include <string.h>
#include "../Headers/parser.h"
#include "../Headers/symbols.h"
#include "../Headers/analyzers.h"
#include "../Headers/opcodes.h"
#include "../Headers/errors.h"
#include "../Headers/globals.h"


int second_pass(FILE *pFile, SymbolNode *pSymbolTable, unsigned int *pCodeImage, ExtNode **pExtList)
{
    char szLine[MAX_LINE_LENGTH];
    ParsedLine parsedLine;
    int nLineNumber = 0;
    int bErrorFound = 0;
    int IC = 100;
    SymbolNode *pTargetSymbol;
    InstructionInfo info;

    /* Rewind the file */
    rewind(pFile);

    while (fgets(szLine, MAX_LINE_LENGTH, pFile) != NULL)
    {
        nLineNumber++;

        /* Parse the line into parts */
        if (parse_line(szLine, &parsedLine, nLineNumber))
        {
            bErrorFound = 1;
            continue;
        }

        /* Skip empty lines and .extern directives */
        if (strlen(parsedLine.szCommand) == 0 || strcmp(parsedLine.szCommand, ".extern") == 0)
        {
            continue;
        }

        /* Handle .entry Directives */
        if (strcmp(parsedLine.szCommand, ".entry") == 0)
        {
            if (!mark_entry(pSymbolTable, parsedLine.szOperands))
            {
                print_error(ERR_LABEL_NOT_FOUND, nLineNumber);
                bErrorFound = 1;
            }
            continue;
        }

        /* Skip Data Directives (Thanks to first pass) */
        if (parsedLine.szCommand[0] == '.')
        {
            continue;
        }

        /* Handle Instruction */
        info = get_instruction_info(parsedLine.szCommand);

        if (info.eType == TYPE_R)
        {
            /* Skip R Type (Thanks to first pass) */
            IC += 4;
        }
        else if (info.eType == TYPE_I)
        {
            /* Change only Branch commands */
            if (info.nOpcode >= 15 && info.nOpcode <= 18)
            {
                {
                char szParsed[3][32];
                extract_operands(parsedLine.szOperands, szParsed);
                pTargetSymbol = find_symbol(pSymbolTable, szParsed[2]);
                if (pTargetSymbol == NULL)
                {
                    print_error(ERR_LABEL_NOT_FOUND, nLineNumber);
                    bErrorFound = 1;
                }
                else
                {
                    int distance = pTargetSymbol->nAddress - IC;
                    
                    /* Update the code to the calculated distance */
                    pCodeImage[(IC - 100) / 4] |= (distance & 0xFFFF);
                }
                }
            }
            IC += 4;
        }
        else if (info.eType == TYPE_J)
        {
            /* Filters out hlt instruction */
            if (info.nOpcode >= 30 && info.nOpcode <= 32)
            {
                char szOps[3][32];
                extract_operands(parsedLine.szOperands, szOps);

                /* Checks if the operand is not a register (then its a label) */
                if (szOps[0][0] != '$')
                {
                    pTargetSymbol = find_symbol(pSymbolTable, szOps[0]);
                    if (pTargetSymbol == NULL)
                    {
                        print_error(ERR_LABEL_NOT_FOUND, nLineNumber);
                        bErrorFound = 1;
                    }
                    else
                    {
                        /* Update the code to the new address */
                        pCodeImage[(IC - 100) / 4] |= (pTargetSymbol->nAddress & 0x3FFFFFF);

                        if (pTargetSymbol->eType == SYMBOL_EXTERN) 
                        {
                            add_extrn_usage(pExtList, pTargetSymbol->szName, IC);
                        }
                    }
                }
            }
            IC += 4;
        }
    }

    return bErrorFound;
}