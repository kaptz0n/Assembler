MAIN: add $3, $5, $9

; Fails because of "garbage_text" at the end
mcro MY_MAC garbage_text
la vall
jmp NEXT
mcroend

mcro MAC_TWO
move $20, $4
; Fails because of "oops" at the end
mcroend oops