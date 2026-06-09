#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/symbols.h"

SymbolNode* add_symbol(SymbolNode **pHead, const char *szName, int nAddress, SymbolType eType)
{
    SymbolNode *pNewNode;
    SymbolNode *pCurrent;

    /* Allocate memory for the symbol */
    pNewNode = (SymbolNode *)malloc(sizeof(SymbolNode));
    if (pNewNode == NULL)
    {
        printf("Error: Failed allocating memory for symbol '%s'\n", szName);
        return NULL;
    }

    /* Initialize the values of the symbol */
    strncpy(pNewNode->szName, szName, 31);
    pNewNode->szName[31] = '\0'; /* Ensure null-termination */
    pNewNode->nAddress = nAddress;
    pNewNode->eType = eType;
    pNewNode->pNext = NULL;

    /* If  list is empty, make this the head of the list */
    if (*pHead == NULL)
    {
        *pHead = pNewNode;
        return pNewNode;
    }

    
    pCurrent = *pHead;
    while (pCurrent->pNext != NULL)
    {
        pCurrent = pCurrent->pNext;
    }
    pCurrent->pNext = pNewNode;

    return pNewNode;
}

SymbolNode* find_symbol(SymbolNode *pHead, const char *szName)
{
    SymbolNode *pCurrent = pHead;

    /* Iterate through the symbollist */
    while (pCurrent != NULL)
    {
        if (strcmp(pCurrent->szName, szName) == 0)
        {
            return pCurrent;
        }
        pCurrent = pCurrent->pNext;
    }

    /* Symbol not found */
    return NULL;
}

void update_data_symbols(SymbolNode *pHead, int nFinalIC)
{
    SymbolNode *pCurrent = pHead;

    /* Adding FIC to data symbols addresses */
    while (pCurrent != NULL)
    {
        if (pCurrent->eType == SYMBOL_DATA)
        {
            pCurrent->nAddress += nFinalIC;
        }
        pCurrent = pCurrent->pNext;
    }
}

void free_symbol_table(SymbolNode *pHead)
{
    SymbolNode *pCurrent;
    
    while (pHead != NULL)
    {
        pCurrent = pHead;
        pHead = pHead->pNext;
        free(pCurrent);
    }
}