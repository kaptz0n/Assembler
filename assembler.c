#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "preprocessor.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *pInputFile;
    FILE *pOutputFile;
    char szInputFilename[256];
    char szOutputFilename[256];
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
        strcpy(szOutputFilename, szInputFilename);
        pDot = strrchr(szOutputFilename, '.'); 
        strcpy(pDot, ".am");

        printf("\n--- Starting compilation for: %s ---\n", szInputFilename);

        pInputFile = fopen(szInputFilename, "r");
        if (pInputFile == NULL)
        {
            printf("Error: Could not open input file '%s'. Skipping.\n", szInputFilename);
            continue;
        }

        pOutputFile = fopen(szOutputFilename, "w");
        if (pOutputFile == NULL)
        {
            printf("Error: Could not create output file '%s'. Skipping.\n", szOutputFilename);
            fclose(pInputFile);
            continue;
        }

        /* Run the Preprocessor */
        if (!process_macros(pInputFile, pOutputFile))
        {
            printf("Preprocessor success! Created expanded file '%s'.\n", szOutputFilename);
            
            /* TODO: First Pass and Second Pass */
        }
        else
        {
            printf("Errors found in macro expansion. Skipping compilation for '%s'.\n", szInputFilename);
            
            fclose(pInputFile);
            fclose(pOutputFile);
            
            /* Delete the corrupted .am file */
            if (remove(szOutputFilename) != 0)
            {
                printf("Warning: Could not delete invalid output file '%s'.\n", szOutputFilename);
            }
            continue; 
        }

        fclose(pInputFile);
        fclose(pOutputFile);
    }

    printf("\nAssembler finished processing all files.\n");
    return 0;
}