; Fails because '$R4' is an register
mcro $R4
sub $3, $2, $1
mcroend

; Fails because it starts with a number
mcro 123MACRO
move $1, $2
mcroend

; Fails because 'entry' is a reserved directive word
mcro entry
la K
mcroend