; bad2.as
DUP:    move $1, $2
DUP:    add $1, $2, $3          ; ERROR: Duplicate label definition

        bne $1, $2, GHOST       ; ERROR: Label 'GHOST' is undefine
        .entry NOLABEL         ; ERROR: Entry declared for non-existent label

add:    move $3, $4             ; ERROR: Reserved word used as a label
        hlt