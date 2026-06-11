; bad1.as
1BADLABEL: add $1, $2, $3       ; ERROR: Label cannot start with a number
GOOD:      add $32, $1, $2      ; ERROR: Register out of bounds (> 31)
           sub $1, $2 $3        ; ERROR: Missing comma between operands
           .asciz "No closing    ; ERROR: String missing closing quote
           .db 5, , 6           ; ERROR: Multiple consecutive commas
           jmp $1, $2           ; ERROR: Too many operands for J-Type