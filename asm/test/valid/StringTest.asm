; This will test .STR directive and Trap 5

;Data
LINE .STR "Hello World\n!"
WORD_ONE .STR "One\t"
WORD_TWO .STR " Two\n"

;Instructions
; LDA to point to array
; read the array
LDA R3 LINE
TRP #5
LDA R3 WORD_ONE
TRP #5
LDA R3 WORD_TWO
TRP #5

;end
TRP #0