; file ps.as
.entry LENGTH
.extern W
.extern L3
MAIN: mov r3 ,LENGTH
LOOP: jmp L1(#-1,r6)
prn #-5
mcr m1
prn #-5
endmcr
bne W(r4,r3)
sub r1, r4
bne L3
m1
L1: inc K
.entry LOOP
bne LOOP(K,W)
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22 