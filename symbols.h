#ifndef SYMBOLS_H
#define SYMBOLS_H

/* Types of symbols */
typedef enum {
    SYMBOL_CODE,  
    SYMBOL_DATA,  
    SYMBOL_EXTERN,
    SYMBOL_ENTRY  
} SymbolType;

/* Structure for the symbol table node*/
typedef struct SymbolNode {
    char szName[32];         
    int nAddress;            
    SymbolType eType;        
    struct SymbolNode *pNext;
} SymbolNode;

/* 
Adds a new symbol to the end of the symbol table.
pHead - Pointer to the head of the symbol list.
szName - The label name to add.
nAddress - The memory address of the label.
eType - The type of symbol (Code, Data, Extern, Entry).
Output:
Returns a pointer to the new node, otherwise NULL.
*/
SymbolNode* add_symbol(SymbolNode **pHead, const char *szName, int nAddress, SymbolType eType);

/* 
Searches the symbol table for a specific label name.
pHead - Pointer to the head of the symbol list.
szName - The label name to search for.
Output:
Returns a pointer to the node if found, otherwise NULL.
 */
SymbolNode* find_symbol(SymbolNode *pHead, const char *szName);

/*
At the end of the First Pass, all data addresses must be updated by 
adding the final Instruction Counter (IC) value to them.
pHead - Pointer to the head of the symbol list.
nFinalIC - The final value of the Instruction Counter after the first pass.
 */
void update_data_symbols(SymbolNode *pHead, int nFinalIC);

/* Frees the entire symbol table memory 
pHead - Pointer to the head of the symbol list.
*/
void free_symbol_table(SymbolNode *pHead);

#endif