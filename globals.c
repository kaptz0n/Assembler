#include "globals.h"

/* List of reserved instructions/opcodes */
const char *g_szReservedWords[] = {
    "add", "sub", "and", "or", "nor", "move", "mvhi", "mvlo",
        "subi", "addi", "andi", "ori", "nori", "bne", "beq", "blt", "bgt",
        "lb", "sb", "lw", "sw", "lh", "sh", "jmp", "la", "call", "hlt",
        "mcro", "mcroend", 
        "db", "dh", "dw", "asciz", "entry", "extern", "data"
};
/* Count of reserved words */
const int g_nReservedWordsCount = sizeof(g_szReservedWords) / sizeof(g_szReservedWords[0]);