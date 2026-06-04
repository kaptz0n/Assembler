; This is a comment, it should be passed to the .am file safely
MAIN: add $3, $5, $9
mcro SWAP_REGS
move $20, $4
move $4, $20
mcroend

  ; This line has leading spaces
LOOP: ori $9, -5, $2
SWAP_REGS
hlt