;Create while loop that counts to user input.
;print every count value

;Instructions used
;TRP 0-3, STR, LDB, MOVI, MOV, CMP, BGT, BLT, BRZ, JMP, ADDI

    ;directive space
INPUT .INT #0 ;set user input
NEWL .BYT '\n' ;new line

;Code space

;get user input
TRP #2 ; read input to r3
STR R3 INPUT ; store input

;now that I know above works lets start looping
LDB R3 NEWL ;
TRP #3 ; write new line

;set registers
LDR R0 INPUT
MOVI R1 #1 ; set counter

;while loop
;R1 is the counter
;R0 is where input will be stored
WHILE LDR R2 INPUT ;tmp store input to compare
CMP R2 R1 ; compare (INPUT, Counter) store result in R2
; IF INPUT > counter R2 = 1, if == then 0, if < then -1
; if input > coutner then run loop
BGT R2 LOOP
; if inpuc == counter
BRZ R2 END
;handle negative input or 0
BLT R2 END

LOOP MOV R3 R1 ; copy counter to R3
TRP #1 ; print counter
MOVI R3 '\n' ;cuz i can
ADDI R1 #1 ; counter ++
TRP #3 ; nl
JMP WHILE


END MOVI R3 '!'
TRP #3
TRP #0 ; end program

