; =========================================
; bad3.as - Macro and Directive Errors
; =========================================
mcro jm1p                        
    add $1, $2, $3
mcroend

        la 500                  ; ERROR: 'la' requires a label, not an integer
        addi $5, LOOP, $6       ; ERROR: 'addi' requires an integer, not a label
        .dh 500.5               ; ERROR: Directive requires integers, not floats
        
LOOP:   hlt