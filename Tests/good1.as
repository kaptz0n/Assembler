; =========================================
; good1.as - The Master Integration Test
; =========================================
.entry MAIN
.extern externalroutine

mcro setupvars
    move $20, $4
    ori $9, -5, $2
mcroend

MAIN:   add $3, $5, $9
        setupvars
LOOP:   bgt $4, $2, ENDPROG
        la K
        sw $0, 4, $10
        bne $31, $9, LOOP
        call externalroutine
        jmp $4

ENDPROG: hlt

STR:    .asciz "Test"
LIST:   .db 6, -9
        .dh 27056
.entry K
K:      .dw 31, -12
hlt