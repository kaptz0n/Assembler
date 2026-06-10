#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../Headers/preprocessor.h"
#include "../Headers/errors.h"
#include "../Headers/globals.h"


MacroNode* add_macro(MacroNode **ppHead, const char *szName)
{
    MacroNode *pNewNode;
    
    pNewNode = (MacroNode*)malloc(sizeof(MacroNode));
    if (pNewNode == NULL)
    {
        return NULL; 
    }
    
    /* Copies macro name and sets pointer to the next node*/
    strcpy(pNewNode->szName, szName);
    pNewNode->pLines = NULL;
    
    pNewNode->pNext = *ppHead;
    *ppHead = pNewNode;
    
    return pNewNode;
}


int add_line_to_macro(MacroNode *pMacro, const char *szLine)
{
    LineNode *pNewLine;
    LineNode *pCurrent;

    if (pMacro == NULL)
    {
        return 1;
    }

    pNewLine = (LineNode*)malloc(sizeof(LineNode));
    if (pNewLine == NULL)
    {
        return 1;
    }
    
    strcpy(pNewLine->szLine, szLine);
    pNewLine->pNext = NULL;

    /* Append to the end of the line list */
    if (pMacro->pLines == NULL)
    {
        pMacro->pLines = pNewLine;
    }
    else
    {
        pCurrent = pMacro->pLines;

        /* Find the last node*/
        while (pCurrent->pNext != NULL)
        {
            pCurrent = pCurrent->pNext;
        }
        pCurrent->pNext = pNewLine;
    }
    return 0;
}


void free_macro_table(MacroNode *pHead)
{
    MacroNode *pCurrentMacro;
    MacroNode *pNextMacro;
    LineNode *pCurrentLine;
    LineNode *pNextLine;

    pCurrentMacro = pHead;
    while (pCurrentMacro != NULL)
    {
        pCurrentLine = pCurrentMacro->pLines;
        
        /* Free all lines */
        while (pCurrentLine != NULL)
        {
            pNextLine = pCurrentLine->pNext;
            free(pCurrentLine);
            pCurrentLine = pNextLine;
        }
        
        /* Free the macro */
        pNextMacro = pCurrentMacro->pNext;
        free(pCurrentMacro);
        pCurrentMacro = pNextMacro;
    }
}


MacroNode* find_macro(MacroNode *pHead, const char *szName)
{
    MacroNode *pCurrentMacro;
    
    pCurrentMacro = pHead;
    while (pCurrentMacro != NULL)
    {
        /* Return the node if the name matches exactly */
        if (strcmp(pCurrentMacro->szName, szName) == 0)
        {
            return pCurrentMacro;
        }
        pCurrentMacro = pCurrentMacro->pNext;
    }
    
    return NULL;
    
}


int is_macro_name_valid(const char *szName)
{
    int i;
    int nRegNum;
    

    if (!isalpha((unsigned char)szName[0]))
    {
        return 0;
    }
    
    for (i = 0; i < g_nReservedWordsCount; i++)
    {
        if (strcmp(szName, g_szReservedWords[i]) == 0)
        {
            return 0;
        }
    }

    if (szName[0] == '$' && isdigit((unsigned char)szName[1]))
    {
        nRegNum = atoi(&szName[1]);
        if (nRegNum >= 0 && nRegNum <= 31)
        {
            return 0;
        }
    }

    return 1;
}


int is_empty_line(const char *szLine)
{
    int i;
    i = 0;
    while (szLine[i] != 0x00)
    {
        if (!isspace((unsigned char)szLine[i]))
        {  
            /* Found a non-whitespace character*/
            return 0; 
        }
        i++;
    }

    /* Line contains only whitespace */
    return 1; 
}


int process_macros(FILE *pInputFile, FILE *pOutputFile)
{
    char szLine[MAX_LINE_LENGTH];
    char w1[MAX_LINE_LENGTH], w2[MAX_LINE_LENGTH], w3[MAX_LINE_LENGTH];
    char *pNewline;
    int c, nTokens, nLineNumber, bErrorFound;

    MacroNode *pMacroTable = NULL;
    MacroNode *pFoundMacro = NULL;
    MacroNode *pCurrentMacro = NULL;
    LineNode *pLineIterator = NULL;

    nLineNumber = 0;
    bErrorFound = 0;

    while (fgets(szLine, MAX_LINE_LENGTH, pInputFile) != NULL)
    {
        nLineNumber++;
        /* Checks for line too long */
        pNewline = strchr(szLine, '\n');
        if (pNewline == NULL && !feof(pInputFile))
        {
            print_error(ERR_LINE_TOO_LONG, nLineNumber);
            bErrorFound = 1;
            while ((c = fgetc(pInputFile)) != '\n' && c != EOF) { }
            continue; 
        }

        /* Skip empty lines */
        if (is_empty_line(szLine))
        {
            fputs(szLine, pOutputFile);
            continue;
        }

        /* Extract up to three words */
        w1[0] = w2[0] = w3[0] = '\0';
        nTokens = sscanf(szLine, "%s %s %s", w1, w2, w3);
        
        if (nTokens <= 0) 
        {
            continue;
        }

        /* Comment Check */
        if (w1[0] == ';')
        {
            fputs(szLine, pOutputFile);
            continue;
        }

        /* Checks if the first field is a macro name */
        pFoundMacro = find_macro(pMacroTable, w1);
        if (pFoundMacro != NULL)
        {
            pLineIterator = pFoundMacro->pLines;
            while (pLineIterator != NULL)
            {
                fputs(pLineIterator->szLine, pOutputFile);
                pLineIterator = pLineIterator->pNext;
            }
            continue; 
        }
        
        /* Checks if the first field is "mcro" */
        else if (strcmp(w1, "mcro") == 0)
        {

            if (nTokens < 2) 
            {
                print_error(ERR_MISSING_MACRO_NAME, nLineNumber);
                bErrorFound = 1;
                continue;
            }
            if (nTokens > 2) 
            {
                print_error(ERR_EXTRA_TEXT, nLineNumber);
                bErrorFound = 1;
                continue;
            }
            if (find_macro(pMacroTable, w2) != NULL)
            {
                print_error(ERR_MACRO_ALREADY_DEFINED, nLineNumber);
                bErrorFound = 1;
                continue;
            }
            if (!is_macro_name_valid(w2)) 
            {
                print_error(ERR_INVALID_MACRO_NAME, nLineNumber);
                bErrorFound = 1;
                continue;
            }
            
            /* Save macro in the table */
            pCurrentMacro = add_macro(&pMacroTable, w2);
            
            /* Read the macro body */
            while (fgets(szLine, MAX_LINE_LENGTH, pInputFile) != NULL)
            {
                nLineNumber++;
                
                pNewline = strchr(szLine, '\n');
                if (pNewline == NULL && !feof(pInputFile))
                {
                    print_error(ERR_LINE_TOO_LONG, nLineNumber);
                    bErrorFound = 1;
                    while ((c = fgetc(pInputFile)) != '\n' && c != EOF) { }
                    continue; 
                }

                w1[0] = w2[0] = '\0';
                sscanf(szLine, "%s %s", w1, w2);

                /* Check if "mcroend" is found */
                if (strcmp(w1, "mcroend") == 0)
                {
                    if (strlen(w2) > 0) 
                    {
                        print_error(ERR_EXTRA_TEXT, nLineNumber);
                        bErrorFound = 1;
                    }

                    break; 
                }
                else
                {
                    add_line_to_macro(pCurrentMacro, szLine);
                }
            }
            continue; 
        }
        else
        {
            fputs(szLine, pOutputFile);
            
        }
    }

    free_macro_table(pMacroTable);
    return bErrorFound;
}