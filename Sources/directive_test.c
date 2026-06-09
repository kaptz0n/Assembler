
#include <stdio.h>
#include <string.h>
#include "../Headers/parser.h"
#include "../Headers/analyzers.h"

/* Helper function to mock up a ParsedLine and run the analyzer */
void run_directive_test(const char *szTestName, const char *szCommand, const char *szOperands)
{
    ParsedLine mockLine;
    unsigned char DataImage[100]; /* Mock memory array */
    int DC = 0;                   /* Data Counter starts at 0 */
    int nResult;
    int i;

    /* 1. Setup the mock package exactly like the Parser would output */
    strcpy(mockLine.szCommand, szCommand);
    strcpy(mockLine.szOperands, szOperands);

    printf("--- %s ---\n", szTestName);
    printf("Line     : [%s %s]\n", mockLine.szCommand, mockLine.szOperands);

    /* 2. Clear memory before test to avoid garbage */
    memset(DataImage, 0, sizeof(DataImage));

    /* 3. Run the function! (Pass 1 as a dummy line number for errors) */
    nResult = process_directive(&mockLine, DataImage, &DC, 1);

    if (nResult == 1)
    {
        printf("Analyzer : REJECTED (Caught an error!)\n\n");
    }
    else
    {
        printf("Analyzer : ACCEPTED (DC is now %d)\n", DC);
        printf("Memory   : ");
        
        /* Print out every byte that was added, formatted in Hexadecimal */
        for (i = 0; i < DC; i++)
        {
            printf("%02X ", DataImage[i]);
        }
        printf("\n\n");
    }
}

int main()
{
    printf("========================================\n");
    printf("      DIRECTIVE ANALYZER TESTS\n");
    printf("========================================\n\n");

    /* --- THE GOOD INPUTS --- */
    
    /* 1 Byte Array: 10 = 0A, -2 = FE */
    run_directive_test(".db Test (1 Byte)", ".db", "10, -2, 5");
    
    /* 2 Byte Array: 300 = 012C (stored as 2C 01), -1 = FFFF */
    run_directive_test(".dh Test (2 Bytes)", ".dh", "300, -1");
    
    /* 4 Byte Array: 100000 = 000186A0 (stored as A0 86 01 00) */
    run_directive_test(".dw Test (4 Bytes)", ".dw", "100000, -2");

    /* String Test: Should print ascii hex values, ending with 00 */
    run_directive_test(".asciz Test", ".asciz", "\"AB C\"");

    /* --- THE BAD INPUTS --- */
    
    /* Missing opening quote */
    run_directive_test("Bad String", ".asciz", "Hello\"");
    
    /* Empty operands */
    run_directive_test("Missing Data", ".db", "");
    
    /* Garbage characters in the number */
    run_directive_test("Garbage Data", ".dw", "15, 20BAD, 5");

    return 0;
}