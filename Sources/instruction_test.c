#include <stdio.h>
#include <string.h>
#include "../Headers/parser.h"
#include "../Headers/analyzers.h"

/* Helper to run a single instruction through the analyzer */
void run_inst_test(const char *szTestName, const char *szCommand, const char *szOperands)
{
    ParsedLine mockLine;
    unsigned int CodeImage[10]; /* Tiny mock code image */
    int IC = 100;               /* Starting IC */
    int nResult;

    /* Setup the mock parsed line */
    strcpy(mockLine.szCommand, szCommand);
    strcpy(mockLine.szOperands, szOperands);



    printf("--- %s ---\n", szTestName);
    printf("Input    : [%s %s]\n", mockLine.szCommand, mockLine.szOperands);

    /* Run the analyzer */
    nResult = process_instruction(&mockLine, CodeImage, &IC, 1);

    if (nResult == 1)
    {
        printf("Analyzer : REJECTED (Caught an error!)\n\n");
    }
    else
    {
        /* %08X prints the 32-bit integer as an 8-character Hexadecimal string */
        printf("Analyzer : ACCEPTED (IC is now %d)\n", IC);
        printf("Machine  : 0x%08X\n\n", CodeImage[0]);
    }
}

int main()
{
    printf("========================================\n");
    printf("    INSTRUCTION ANALYZER TESTS\n");
    printf("========================================\n\n");

    /* 1. R-Type (Opcode 0, rs=3, rt=5, rd=9, funct=1)  */ 
    run_inst_test("R-Type (Math)", "add", "$3, $5, $9");

     /* 2. R-Type Copy (Opcode 1, rs=20, rt=0, rd=4, funct=1) */
    run_inst_test("R-Type (Copy)", "move", "$20, $4");

     /* 3. I-Type Math (Opcode 10, rs=9, rt=2, immed=-5) -> -5 should mask to FFFB */
    run_inst_test("I-Type (Math Immed)", "addi", "$9, -5, $2");

    /* 4. I-Type Branch (Opcode 15, rs=31, rt=9, immed=0 for now) */
    run_inst_test("I-Type (Branch Label)", "bne", "$31, $9, LOOP");
    
    /* 5. J-Type Register (Opcode 30, reg=1, rs=4) */
    run_inst_test("J-Type (Register)", "jmp", "$4");

    /* 6. J-Type Label (Opcode 30, reg=0, immed=0 for now) */
    run_inst_test("J-Type (Label)", "jmp", "NEXT");

    /* 7. J-Type Halt (Opcode 63, no operands) */
    run_inst_test("J-Type (Halt)", "hlt", "");

     /* --- THE BAD INPUTS --- */
    run_inst_test("Syntax Error (Too many ops)", "hlt", "$3");
    run_inst_test("Invalid Register Name", "add", "$32, $5, $9");
    run_inst_test("Unknown Command", "fakecmd", "$1");

    return 0;
}