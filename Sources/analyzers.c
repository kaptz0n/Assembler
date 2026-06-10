#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Headers/analyzers.h"
#include "../Headers/errors.h"
#include "../Headers/globals.h"
#include "../Headers/opcodes.h"


/*
Extracts the register number from a string.
szOperand - The operand string.
Output:
Returns the register number (0-31) on success, -1 for an invalid register.
*/
static int get_register_number(const char *szOperand)
{
    int nReg;
    char *pEnd;

    /* Checks that the operand starts with '$' */
    if (szOperand[0] != '$') 
    {
        return -1;
    }

    /* Convert to a number */
    nReg = strtol(szOperand + 1, &pEnd, 10);

    /* Checks for invalid input */
    if (*pEnd != '\0' || nReg < 0 || nReg > 31)
    {
        return -1;
    }

    return nReg;
}


int process_directive(ParsedLine *pLine, unsigned char *pDataImage, int *pDC, int nLineNumber)
{
    char *pToken;
    char szOperandsCopy[MAX_LINE_LENGTH];
    long nVal;
    char *pEnd;
    int i, nLen;

    /* Handle Strings */
    if (strcmp(pLine->szCommand, ".asciz") == 0)
    {
        nLen = strlen(pLine->szOperands);
        
        /* Checks if the string starts and ends with quotes */
        if (nLen < 2 || pLine->szOperands[0] != '"' || pLine->szOperands[nLen - 1] != '"')
        {
            print_error(ERR_STRING_MISSING_QUOTES, nLineNumber);
            return 1;
        }

        /* Copies the string to the Data Image */
        for (i = 1; i < nLen - 1; i++)
        {
            pDataImage[*pDC] = pLine->szOperands[i];
            (*pDC)++;
        }
        
        /* Terminating the string in the Data Image */
        pDataImage[*pDC] = '\0';
        (*pDC)++;
        
        return 0;
    }

    /* Handle Numbers */
    if (strcmp(pLine->szCommand, ".db") == 0 || 
        strcmp(pLine->szCommand, ".dh") == 0 || 
        strcmp(pLine->szCommand, ".dw") == 0)
    {
        strcpy(szOperandsCopy, pLine->szOperands);

        pToken = strtok(szOperandsCopy, ",");
        if (pToken == NULL)
        {
            print_error(ERR_MISSING_DATA, nLineNumber);
            return 1;
        }

        while (pToken != NULL)
        {
            /* Converts to number  */
            nVal = strtol(pToken, &pEnd, 10);
            
            /* Skips whitespaces */
            while (*pEnd != '\0' && isspace((unsigned char)*pEnd))
            {
                pEnd++;
            }
            
            /* If pEnd is not pointing to the null terminator, there was garbage text */
            if (*pEnd != '\0')
            {
                print_error(ERR_INVALID_NUMBER_FORMAT, nLineNumber);
                return 1;
            }

            /* Copies the number to the Data Image based on its size */
            if (strcmp(pLine->szCommand, ".db") == 0) 
            {
                /* 1 Byte */
                pDataImage[*pDC] = (unsigned char)(nVal & 0xFF); 
                (*pDC) += 1;
            }
            else if (strcmp(pLine->szCommand, ".dh") == 0) 
            {
                /* 2 Bytes (Half-Word) */
                pDataImage[*pDC]     = (unsigned char)(nVal & 0xFF);         
                pDataImage[*pDC + 1] = (unsigned char)((nVal >> 8) & 0xFF);  
                (*pDC) += 2;
            }
            else if (strcmp(pLine->szCommand, ".dw") == 0) 
            {
                /* 4 Bytes (Word) */
                pDataImage[*pDC]     = (unsigned char)(nVal & 0xFF);         
                pDataImage[*pDC + 1] = (unsigned char)((nVal >> 8) & 0xFF);  
                pDataImage[*pDC + 2] = (unsigned char)((nVal >> 16) & 0xFF); 
                pDataImage[*pDC + 3] = (unsigned char)((nVal >> 24) & 0xFF); 
                (*pDC) += 4;
            }

            pToken = strtok(NULL, ",");
        }
        return 0;
    }

    /* No matching directive was found */
    print_error(ERR_UNKNOWN_DIRECTIVE, nLineNumber);
    return 1; 
}


int process_instruction(ParsedLine *pLine, unsigned int *pCodeImage, int *pIC, int nLineNumber)
{
    InstructionInfo info;
    char szOps[3][32];
    int nOpCount;
    unsigned int nMachineWord = 0;
    int rs = 0, rt = 0, rd = 0;
    long immed = 0;
    char *pEnd;

    /* Look up the command in the Opcode Dictionary */
    info = get_instruction_info(pLine->szCommand);
    if (info.eType == TYPE_UNKNOWN)
    {
        print_error(ERR_UNKNOWN_COMMAND, nLineNumber);
        return 1;
    }
    
    /* Slice the operands and verify the count */
    nOpCount = extract_operands(pLine->szOperands, szOps);
    
    if (nOpCount != info.nExpectedOperands)
    {
        print_error(ERR_WRONG_OPERAND_COUNT, nLineNumber);
        return 1;
    }
    /* Build the 32-bit machine code */
    switch (info.eType)
    {
        case TYPE_R:
            rs = get_register_number(szOps[0]);
            
            /* Arithmetic operations */
            if (info.nExpectedOperands == 3) 
            {
                rt = get_register_number(szOps[1]);
                rd = get_register_number(szOps[2]);
            } 
            /* Move operations */
            else 
            {
                rd = get_register_number(szOps[1]);
            }

            /* Checks for invalid registers */
            if (rs == -1 || rt == -1 || rd == -1) {
                print_error(ERR_INVALID_REGISTER, nLineNumber);
                return 1;
            }

            /* Shifts everything into its exact bit position */
            nMachineWord = (info.nOpcode << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (info.nFunct << 6);
            break;

        case TYPE_I:
            rs = get_register_number(szOps[0]);
            
            /* Checks if the instruction is a branch */
            if (szOps[1][0] == '$') 
            { 
                rt = get_register_number(szOps[1]);
                immed = strtol(szOps[2], &pEnd, 10);
            }

            /* Not a branch */
            else 
            {
                immed = strtol(szOps[1], &pEnd, 10);
                rt = get_register_number(szOps[2]);
            }

            /* Checks for invalid registers */
            if (rs == -1 || rt == -1) {
                print_error(ERR_INVALID_REGISTER, nLineNumber);
                return 1;
            }

            /* Check if immed is a number or a label */
            if (*pEnd != '\0') 
            {
                /* Checks if the label name is valid */
                    if (!is_valid_label(szOps[2])) {
                        print_error(ERR_INVALID_LABEL, nLineNumber);
                        return 1;
                    }
                immed = 0;
            }

            /* It's a number */
            else
            {
                /* Mask it to 16 bits */
                immed = immed & 0xFFFF;
            }

            nMachineWord = (info.nOpcode << 26) | (rs << 21) | (rt << 16) | immed;
            break;

        case TYPE_J:
            
            /* Checks if the instruction is hlt */
            if (info.nExpectedOperands == 0) 
            {
                nMachineWord = (info.nOpcode << 26);
            }
            /* Its a jmp/call/la */
            else
            {
                /* Checks if the first operand is a register */
                if (szOps[0][0] == '$') {
                    
                    rs = get_register_number(szOps[0]);

                    /* Checks for invalid register */
                    if (rs == -1) {
                        print_error(ERR_INVALID_REGISTER, nLineNumber);
                        return 1;
                    }
                    nMachineWord = (info.nOpcode << 26) | (1 << 25) | rs;

                }
                /* Not a register, than it's a label */
                else
                {
                    /* Checks if the label name is valid */
                    if (!is_valid_label(szOps[0])) {
                        print_error(ERR_INVALID_LABEL, nLineNumber);
                        return 1;
                    }
                    nMachineWord = (info.nOpcode << 26); 
                }
            }
            break;
            
        case TYPE_UNKNOWN:
            break;
    }

    
    /* Save to Code Image and advance IC */
    pCodeImage[(*pIC - 100) / 4] = nMachineWord;
    *pIC += 4;

    return 0;
}