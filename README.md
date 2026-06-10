# Assembler #

This project takes an assembly file and produces machine code object file.
The project is divided to 3 parts:
    1. Preprocessor: This part checks for macro definitions in the assembly file and copies their content to line where the macro is mentioned
    2. First Pass: This part creates a symbol table for every symbol in the assembly file (except .entry) and starts to create the code and data image of the object file.
    3. Second Pass: This part finishes the job of creating the code and data image, it resolves every symbol mentioned in the file and fills its address every time an instruction mentions it
Finally We're left with an object file (.ob),for each assembly file. and if needed an extern file (.ext), an entry file (.ent).
Object file - contains the code and data image.
Extern file - contains the address of where the external symbol was mentioned.
Entry file - contains the address of all the entry symbols.