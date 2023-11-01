;Create while loop that counts to user input.
;on every value divisible by 5 print !
;print every count value

;directive space
INPUT .INT #0 ;set user input
NEWL .BYT '\n' ;new line

;Code space
TRP #2 ; read input to r3
STR R3 INPUT ; store input
MOVI R3 #0 ; zero at register for testing
TRP #1 ; write from r3. should print 0
LDR R3 INPUT ;load input
TRP #1 ; write from r3. should print input

;now that I know above works lets start looping
LDB R3 NEWL ;
TRP #3 ; write new line

;set registers
LDR R0 INPUT
MOVI R1 #0 ; set counter

;while loop
;R1 is the counter
;R0 is where input will be stored
WHILE LDR R2 INPUT ;tmp store input to compare
CMP R2 R1 ; compare input with counter. store result in R1


TRP #0 ; end program

