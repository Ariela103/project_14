; file ps.am
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
macro m1
 inc r6
 mov r3, W
endm
lea STR, r6
inc val1
mov r3, W
sub r1, r4
bne END
bne END[r15]
dec val1
.entry MAIN
sub LOOP[r10] ,W
END: stop
STR: .string "abcd"
LIST: .data 6, -9
.data -100
.entry K
K: .data 31
.extern val1