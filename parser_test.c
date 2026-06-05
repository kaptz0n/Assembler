#include <stdio.h>
#include <string.h>
#include "parser.h"


void run_test(const char *szTestName, const char *szRawLine)
{
    ParsedLine output;
    int nResult;

    printf("--- %s ---\n", szTestName);
    printf("Raw text : [%s]\n", szRawLine);
    

    nResult = parse_line(szRawLine, &output, 67);

    if (nResult == 1)
    {
        printf("Parser   : REJECTED (Caught an error!)\n\n");
    }
    else
    {
        printf("Parser   : ACCEPTED\n");
        printf("Label    : [%s]\n", output.szLabel);
        printf("Command  : [%s]\n", output.szCommand);
        printf("Operands : [%s]\n\n", output.szOperands);
    }
}

int main()
{
    printf("========================================\n");
    printf("        PARSER UNIT TESTS\n");
    printf("========================================\n\n");

    /* --- GOOD INPUTS --- */
    run_test("Standard Line", "MAIN: add $6, $7");
    
    run_test("No Label, Lots of Spaces", "     .db     6, 7  ");
    
    run_test("String (Checking if spaces are preserved)", "STR: .asciz \"67676767\"");
    
    run_test("Comment Line", "; This should be completely ignored");

    /* --- BAD INPUTS --- */
    run_test("Reserved Word Label", "add: move $1, $2");
    
    run_test("Number First Label", "1BAD: jmp MAIN");

    run_test("Too Long Label", "THIS_LABEL_IS_WAY_TOO_LONG_FOR_X: hlt");

    return 0;
}