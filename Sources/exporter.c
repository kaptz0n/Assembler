#include <stdio.h>
#include <string.h>
#include "../Headers/symbols.h"
#include "../Headers/globals.h"


void change_extension(char *szFilename, const char *szNewExt)
{
    char *pDot = strrchr(szFilename, '.');
    if (pDot != NULL)
    {
        strcpy(pDot, szNewExt);
    }
}

void export_files(const char *szOriginalFilename, int nFinalIC, int nFinalDC, unsigned int *pCodeImage, unsigned char *pDataImage, SymbolNode *pSymbolTable, ExtNode *pExtList)
{
    FILE *pFile;
    char szFilename[256];
    int i, nAddress;
    SymbolNode *pSymTemp;
    ExtNode *pExtTemp;
    int bHasEntries = 0;

    /* Export the .ob file */
    strcpy(szFilename, szOriginalFilename);
    change_extension(szFilename, ".ob");
    
    pFile = fopen(szFilename, "w");
    if (pFile != NULL)
    {
        /* Start of the file: total IC and DC */
        fprintf(pFile, "%d %d\n", nFinalIC - 100, nFinalDC);

        /* Print Code Image */
        nAddress = 100;
        for (i = 0; i < (nFinalIC - 100) / 4; i++)
        {
            unsigned int word = pCodeImage[i];

            /* Little-Endian order */
            fprintf(pFile, "%04d %02X %02X %02X %02X\n", 
                    nAddress, 
                    (word & 0xFF), 
                    ((word >> 8) & 0xFF), 
                    ((word >> 16) & 0xFF), 
                    ((word >> 24) & 0xFF));
            nAddress += 4;
        }

        /* Print Data Image */
        for (i = 0; i < nFinalDC; i += 4)
        {
            fprintf(pFile, "%04d", nAddress);
            
            /* Print up to 4 bytes on every line */
            if (i < nFinalDC)
            {
                fprintf(pFile, " %02X", pDataImage[i]);
            }
            if (i + 1 < nFinalDC)
            {
                fprintf(pFile, " %02X", pDataImage[i + 1]);
            }
            if (i + 2 < nFinalDC)
            {
                fprintf(pFile, " %02X", pDataImage[i + 2]);
            }
            if (i + 3 < nFinalDC)
            {
                fprintf(pFile, " %02X", pDataImage[i + 3]);
            }

            fprintf(pFile, "\n");
            nAddress += 4;
        }
        fclose(pFile);
    }

    /* Export the .ent file */
    /* Check for symbols marked as entries */
    pSymTemp = pSymbolTable;
    while (pSymTemp != NULL)
    {
        if (pSymTemp->isEntry) 
        {
            bHasEntries = 1;
        }
        pSymTemp = pSymTemp->pNext;
    }

    /* Creates the file if there are entries */
    if (bHasEntries)
    {
        strcpy(szFilename, szOriginalFilename);
        change_extension(szFilename, ".ent");
        
        pFile = fopen(szFilename, "w");
        if (pFile != NULL)
        {
            pSymTemp = pSymbolTable;
            while (pSymTemp != NULL)
            {
                if (pSymTemp->isEntry)
                {
                    fprintf(pFile, "%s %04d\n", pSymTemp->szName, pSymTemp->nAddress);
                }
                pSymTemp = pSymTemp->pNext;
            }
            fclose(pFile);
        }
    }

    /* Export the .ext file */
    /* Create the file if there are external symbols that were used */
    if (pExtList != NULL)
    {
        strcpy(szFilename, szOriginalFilename);
        change_extension(szFilename, ".ext");
        pFile = fopen(szFilename, "w");
        if (pFile != NULL)
        {
            pExtTemp = pExtList;
            while (pExtTemp != NULL)
            {
                fprintf(pFile, "%s %04d\n", pExtTemp->szName, pExtTemp->nAddress);
                pExtTemp = pExtTemp->pNext;
            }
            fclose(pFile);
        }
    }
}