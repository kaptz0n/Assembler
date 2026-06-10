#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/errors.h"
#include "../Headers/preprocessor.h"
#include "../Headers/symbols.h"
#include "../Headers/first_pass.h"
#include "../Headers/second_pass.h"
#include "../Headers/globals.h"
#include "../Headers/exporter.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *pInputFile;
    FILE *pOutputFile;
    char szInputFilename[256];
    char szAmFile[256];
    char *pDot;

    /* Checks for at least one file */
    if (argc < 2)
    {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    /* Assemble each file separately */
    for (i = 1; i < argc; i++)
    {
        strcpy(szInputFilename, argv[i]);
        
        /* Find the last dot to manage extensions*/
        pDot = strrchr(szInputFilename, '.');

       /* Check if its an ".as" file */
        if (pDot == NULL || strcmp(pDot, ".as") != 0)
        {
            printf("Error: File '%s' does not have a '.as' extension. Skipping.\n", szInputFilename);
            continue;
        }
        
        /* Create the .am output filename  */
        strcpy(szAmFile, szInputFilename);
        change_extension(szAmFile, ".am");

        printf("\n--- Starting compilation for: %s ---\n", szInputFilename);

        pInputFile = fopen(szInputFilename, "r");
        if (pInputFile == NULL)
        {
            printf("Error: Could not open input file '%s'. Skipping.\n", szInputFilename);
            continue;
        }

        pOutputFile = fopen(szAmFile, "w");
        if (pOutputFile == NULL)
        {
            printf("Error: Could not create output file '%s'. Skipping.\n", szAmFile);
            fclose(pInputFile);
            continue;
        }

        /* Runs the Preprocessor */
        if (!process_macros(pInputFile, pOutputFile))
        {
            FILE *pAmFile;
            SymbolNode *pSymbolTable = NULL;
            unsigned char DataImage[MAX_IMAGE_SIZE] = {0};
            unsigned int CodeImage[MAX_IMAGE_SIZE] = {0};
            ExtNode *pExtList = NULL;
            int finalDC = 0;
            int finalIC = 0;

            printf("Preprocessor success! Created expanded file '%s'.\n", szAmFile);
            
            fclose(pOutputFile);

            /* Prevent double-closing at the bottom of the loop */
            pOutputFile = NULL; 

            pAmFile = fopen(szAmFile, "r");
            if (pAmFile == NULL)
            {
                printf("Error: Could not open '%s' for First Pass. Skipping.\n", szAmFile);
                fclose(pInputFile);
                continue;
            }

            printf("Starting First Pass.\n");
            if (!first_pass(pAmFile, &pSymbolTable, DataImage, CodeImage, &finalDC, &finalIC))
            {
                printf("First Pass success!\nStarting Second Pass.\n");
                if (!second_pass(pAmFile, pSymbolTable, CodeImage, &pExtList))
                {
                    printf("Second Pass success! Memory addresses resolved.\n");
                    export_files(szInputFilename, finalIC, finalDC, CodeImage, DataImage, pSymbolTable, pExtList);

                }
                else
                {
                    printf("Errors found during Second Pass. Compilation aborted for '%s'.\n", szInputFilename);
                }
            }
            else
            {
                printf("Errors found during First Pass. Compilation aborted for '%s'.\n", szInputFilename);
            }

            /* Clean up First Pass resources for this specific file */
            fclose(pAmFile);
            free_symbol_table(pSymbolTable);
            free_extrn_list(pExtList);
        }
        else
        {
            printf("Errors found in macro expansion. Skipping compilation for '%s'.\n", szInputFilename);
            
            fclose(pInputFile);
            pInputFile = NULL;
            fclose(pOutputFile);
            pOutputFile = NULL;

            /* Delete the corrupted .am file */
            if (remove(szAmFile) != 0)
            {
                printf("Warning: Could not delete invalid output file '%s'.\n", szAmFile);
            }
            continue; 
        }

        if (pInputFile != NULL) 
        {
            fclose(pInputFile);
        }
        if (pOutputFile != NULL) 
        {
            fclose(pOutputFile);
        }
    }

    printf("\nAssembler finished processing all files.\n");
    return 0;
}