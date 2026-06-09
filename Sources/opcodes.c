#include <string.h>
#include "../Headers/opcodes.h"

/* 
 All R, I, and J instructions in our architecture, along with their profiles (opcode, funct, expected operands).
 */
static const InstructionInfo g_OpcodeTable[] = {
    {"add",  TYPE_R, 0, 1, 3},
    {"sub",  TYPE_R, 0, 2, 3},
    {"and",  TYPE_R, 0, 3, 3},
    {"or",   TYPE_R, 0, 4, 3},
    {"nor",  TYPE_R, 0, 5, 3},
    {"move", TYPE_R, 1, 1, 2},
    {"mvhi", TYPE_R, 1, 2, 2},
    {"mvlo", TYPE_R, 1, 3, 2},
    {"addi", TYPE_I, 10, 0, 3},
    {"subi", TYPE_I, 11, 0, 3},
    {"andi", TYPE_I, 12, 0, 3},
    {"ori",  TYPE_I, 13, 0, 3},
    {"nori", TYPE_I, 14, 0, 3},
    {"bne",  TYPE_I, 15, 0, 3},
    {"beq",  TYPE_I, 16, 0, 3},
    {"blt",  TYPE_I, 17, 0, 3},
    {"bgt",  TYPE_I, 18, 0, 3},
    {"lb",   TYPE_I, 19, 0, 3},
    {"sb",   TYPE_I, 20, 0, 3},
    {"lw",   TYPE_I, 21, 0, 3},
    {"sw",   TYPE_I, 22, 0, 3},
    {"lh",   TYPE_I, 23, 0, 3},
    {"sh",   TYPE_I, 24, 0, 3},
    {"jmp",  TYPE_J, 30, 0, 1},
    {"la",   TYPE_J, 31, 0, 1},
    {"call", TYPE_J, 32, 0, 1},
    {"hlt",  TYPE_J, 63, 0, 0}
};

/* Count of instructions */
static const int g_nOpcodeTableSize = sizeof(g_OpcodeTable) / sizeof(g_OpcodeTable[0]);

InstructionInfo get_instruction_info(const char *szCommand)
{
    int i;
    InstructionInfo unknownInfo;

    /* Search for the instruction */
    for (i = 0; i < g_nOpcodeTableSize; i++)
    {
        if (strcmp(szCommand, g_OpcodeTable[i].szName) == 0)
        {
            return g_OpcodeTable[i]; 
        }
    }

    /* Instruction doesn't exist */
    strcpy(unknownInfo.szName, "unknown");
    unknownInfo.eType = TYPE_UNKNOWN;
    unknownInfo.nOpcode = 0;
    unknownInfo.nFunct = 0;
    unknownInfo.nExpectedOperands = 0;
    return unknownInfo;
}