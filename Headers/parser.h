#ifndef PARSER_H
#define PARSER_H

/* Struct for parsed line */
typedef struct {
    char szLabel[32];      
    char szCommand[32];    
    char szOperands[80];   
} ParsedLine;


/*
Validates a label name.
szName - The name of the label to validate.
Output:
1 valid, 0 if invalid.
 */
int is_valid_label(const char *szName);


/*
Takes a raw text line, extracts the parts, validates the label, 
and fills the ParsedLine struct.
szLine - The raw line of text from the source file.
pOutput - Pointer to the struct to fill with the parsed components.
nLineNumber - The line number in the source file (for error reporting).
Output:
Return 0 on success, nonzero on failure.
 */
int parse_line(const char *szLine, ParsedLine *pOutput, int nLineNumber);


/* 
Extracts operands from a comma-separated string and removes spaces.
szRawOperands - The raw operand string.
szParsed - A 2D array to hold up to 3 cleaned operand strings.
Output:
Returns the number of operands extracted.
*/
int extract_operands(const char *szRawOperands, char szParsed[3][32]);


#endif
