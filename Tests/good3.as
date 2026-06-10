; =========================================
; good3.as - Boundary and Edge Cases
; =========================================
.extern EXTLABEL

MaxLenLabelExact31Characters: .dh 0, 32767, -32768

    add $0, $0, $0
    sub $31, $31, $31

    call EXTLABEL
    jmp MaxLenLabelExact31Characters

    hlt