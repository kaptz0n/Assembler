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
    int isEntry; 
    struct SymbolNode *pNext;
} SymbolNode;

/* Structure for logging extern labels that are being used in the code */
typedef struct ExtNode {
    char szName[32];
    int nAddress;
    struct ExtNode *pNext;
} ExtNode;


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
Adds the FIC to all data symbols after the first pass.
pHead - Pointer to the head of the symbol list.
nFinalIC - The final value of the Instruction Counter after the first pass.
 */
void update_data_symbols(SymbolNode *pHead, int nFinalIC);


/* Frees the entire symbol table memory 
pHead - Pointer to the head of the symbol list.
*/
void free_symbol_table(SymbolNode *pHead);


/*
Marks a symbol as an entry.
pHead - Pointer to the head of the symbol list.
szName - The label name to mark as entry.
Output:
Returns 1 if the symbol was found and marked, otherwise 0.
 */
int mark_entry(SymbolNode *pHead, const char *szName);


/*
Adds a record of an external label usage to the linked list.
pHead - Pointer to the head of the extern list.
szName - The label name to add.
nAddress - The address that the label is used at.
 */
void add_extrn_usage(ExtNode **pHead, const char *szName, int nAddress);


/*
Frees the entire extern list memory.
pHead - Pointer to the head of the extern list.*/
void free_extrn_list(ExtNode *pHead);


#endif
