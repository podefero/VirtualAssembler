;use while loop to sum an array of ints
;each iteration check array index is even or odd, print.
;print the result and end
;DATA
SIZE .INT #10 ;size of array
ARR .INT #10 ; array
.INT #2
.INT #3
.INT #4
.INT #15
.INT #-6
.INT #7
.INT #8
.INT #9
.INT #10
INDEX .INT #0 ;index
sum .INT #0 ;sum variable
temp .INT ; temp variable
result .INT ; sum result variable




;CODE
;set registers
LDA R0, ARR ; R0 for array pointer
LDR R1, SIZE ; R1 for size

;while(INDEX<SIZE)
WHILE LDR R2, INDEX ; load INDEX in R2 for comparing
CMP R2, R1 ; compare (INDEX,size) store rsult in R2
; if R2 is -1 then loop
BLT R2 LOOP
; if R2 is 0 then print result
BRZ R2 END

;LOOP.
;inc INDEX and update
LOOP LDR R4 INDEX ; get INDEX
ADDI R4 #1 ; inc INDEX
STR R4 INDEX ; update INDEX
;get array, sum, add arr[INDEX] to sum and update arr pointer
LDR R4 R0 ; get arr[INDEX]
STR R4 result ; store arr[INDEX] in result

; check if result is even or odd. use temp to save result
MOV R15 R16 ;store pc in R15 so we can return here
ADDI R15 #24 ;offset the return so we don't repeat sub-routine
JMP MOD ;run mod function

; if temp is 0 it's even, if 1 it's odd
LDR R4 temp ; get temp
MOV R15 R16 ; copy pc
ADDI R15 #36 ; offset to LDR
BRZ R4 PRINT_EVEN ; print is even
BGT R4 PRINT_ODD ; print is odd
LDR R4 R0 ; get arr[INDEX] again
ADDI R0 #4 ; inc arr pointer
LDR R5 sum ; get sum
ADD R5 R4 ; add sum arr[INDEX]
STR R5 sum ; update sum

; return to loop
JMP WHILE

MOD LDR R4 result ; get result
MOV R6 R4 ; copy result to R6
MOVI R5 #2 ; get 2 for modulus 2
DIV R4 R5 ; result / 2
MUL R4 R5 ; result * 2
SUB R6 R4 ; result - R4
STR R6 temp ; store temp
JMR R15 ; return

; print # is even \n
PRINT_EVEN LDR R3 result ; get arr[i]
TRP #1 ; print result
MOVI R3 ' '
TRP #3 ; print space
MOVI R3 'i'
TRP #3 ; print i
MOVI R3 's'
TRP #3 ; print s
MOVI R3 ' '
TRP #3 ; print space
MOVI R3 'e'
TRP #3 ' ; print e
MOVI R3 'v'
TRP #3 ; print v
MOVI R3 'e'
TRP #3
MOVI R3 'n'
TRP #3
MOVI R3 '\n'
TRP #3
JMR R15 ; return

; print # is odd \n
PRINT_ODD LDR R3 result ; get arr[i]
TRP #1 ; print result
MOVI R3 ' '
TRP #3 ; print space
MOVI R3 'i'
TRP #3
MOVI R3 's'
TRP #3
MOVI R3 ' '
TRP #3
MOVI R3 'o'
TRP #3 '
MOVI R3 'd'
TRP #3
MOVI R3 'd'
TRP #3
MOVI R3 '\n'
TRP #3
JMR R15 ; return

; print sum is #
PRINT_SUM MOVI R3 'S' ; get arr[i]
TRP #3 ; print result
MOVI R3 'u'
TRP #3 ; print space
MOVI R3 'm'
TRP #3
MOVI R3 ' '
TRP #3
MOVI R3 'i'
TRP #3
MOVI R3 's'
TRP #3 '
MOVI R3 ' '
TRP #3
LDR R3 sum
TRP #1
MOVI R3 '\n'
TRP #3
JMR R15 ; return

;Print sum
END MOV R15 R16 ; set pc to return
ADDI R15 #24 ; offset return
JMP PRINT_SUM ; print sum
MOVI R3 ''
TRP #3
TRP #0 ; end