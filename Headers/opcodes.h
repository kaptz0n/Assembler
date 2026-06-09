#ifndef OPCODES_H
#define OPCODES_H

/* Instruction types */
typedef enum {
    TYPE_R,
    TYPE_I,
    TYPE_J,
    TYPE_UNKNOWN /* Instruction doesn't exist */
} InstructionType;

/* Instruction Structure */
typedef struct {
    char szName[10];
    InstructionType eType;
    unsigned int nOpcode;
    unsigned int nFunct;
    int nExpectedOperands;
} InstructionInfo;

/*
Looks up a command string in the dictionary.
szCommand - The command string to look up (e.g., "add", "lw").
Output:
Returns the InstructionInfo struct. If not found, eType will be TYPE_UNKNOWN.
 */
InstructionInfo get_instruction_info(const char *szCommand);

#endif