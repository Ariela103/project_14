;this file will include errors- using of wrong addresing methods

;wrong addressing method for move
mov #3, #-6
LABEL1: .data 6, -17, 5   ,  2,3
LABEL2: cmp  LABEL1, r5
;wrong source operand, registery number out of range
add r9  ,#7

;wrong target operand, illegal target operand
add r2  ,*r3

;wrong target addressing method for sub
sub r5   ,   #1
;wrong addressing method for lea
lea #52 , r3
;wrong addressing method for lea
lea LABEL2 m #99
;wrong addressing method for lea (for both operands)
lea #4,#1
;wrong addressing method for clr
   clr #-67
    ;wrong addressing method for not
not       #-536
;wrong addressing method for inc
 inc #6
  ;wrong addressing method for dec 
           dec #8
;wrong addressing method for jmp
jmp #2
;wrong addressing method for jmp
jmp    r5
.extern LABEL2
    ;wrong addressing method for bne
bne   r6
;wrong addressing method for bne
 bne    #-901
;wrong addressing method for jsr
jsr r10
;wrong addressing method for jsr
jsr #2
;wrong addressing method for red
    red     #-80