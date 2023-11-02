;compare DAGS and GADS
;DATA
DAGS .BYT 'D' ; char array
.BYT 'A'
.BYT 'G'
.BYT 'S'
GADS .INT #-99
rel .BYT ;

;CODE
;set each GADS byte
LDA R2 DAGS ; get pointer to DAGS
LDA R1 GADS ; get pointer to GADS
ADDI R2 #2 ; get second index
LDB R2 R2 ; get DAGS[2]
STR R2 R1 ; store DAGS[2] in address GADS[0]

;now the next [1]
LDA R2 DAGS ; get pointer to GADS
ADDI R2 #1 ; move index to 1
ADDI R1 #1 ; move GADS index to 1
LDB R2 R2 ; get DAGS[1]
STR R2 R1 ; store in address of GADS[1]

;now [2]
LDA R2 DAGS ; get pointer to GADS
ADDI R1 #1 ; move GADS index to 2
LDB R2 R2 ; get DAGS[0]
STR R2 R1 ; store in address of GADS[1]

;now the last [3]
LDA R2 DAGS ; get pointer to GADS
ADDI R2 #3 ; move index to 3
ADDI R1 #1 ; move GADS index to 3
LDB R2 R2 ; get DAGS[3]
STR R2 R1 ; store in address of GADS[3]

;R0 points to DAGS (char array)
;R1 points to GADS (int)
;R2 is counter (j)
;R3 is CMPI result
LDA R0 DAGS
LDA R1 GADS
MOVI R2 #0 ; set j
MOVI R3 #0

FOR MOV R3 R2 ; set R3 to j for comparing
MOVI R12 #12 ; instruction size
CMPI R3 #4 ; compare(j, 4) store result in R3
ADDI R2 #1 ; inc j
; branch
BLT R3 COMPARE ; if j < 4 jump to COMPARE
BRZ R3 END ; if j==4 end

COMPARE LDB R3 R0 ; get DAGS[j]
    LDB R4 R1 ; get GADS[j]
    CMP R3 R4 ; compare the arrays
    BLT R3 LESST ; set rel to <
    BGT R3 GREATT ; set rel to >
    BRZ R3 EQUALT ; set rel to =

    LESST MOVI R3 '<'
        STB R3 rel ; store < in rel
        JMP PRINTLOOP
    GREATT MOVI R3 '>'
        STB R3 rel ; store > in rel
        JMP PRINTLOOP
    EQUALT MOVI R3 '='
        STB R3 rel ; store = in rel
        JMP PRINTLOOP

; print "DAGS[j] rel GADS[j] -- "
    PRINTLOOP LDB R3 R0
    TRP #3
; print DAGS[j]
    MOVI R3 ' '
    TRP #3
; space
    LDB R3 rel
    TRP #3
; rel
    MOVI R3 ' '
    TRP #3
; space
    LDB R3 R1
    TRP #3
; GADS[j]
    MOVI R3 ' '
    TRP #3
; space
    MOVI R3 '-'
    TRP #3
    TRP #3
; --
    MOVI R3 ' '
    TRP #3
;update array offsets
    ADDI R0 #1 ; update offset DAGS
    ADDI R1 #1 ; update offset GADS
JMP FOR ; return to for loop


END MOVI R3 '\n'
TRP #3
TRP #3
; new line twice

LDR R0 DAGS ;get DAGS as int
LDR R1 GADS
MOV R4 R0 ; copy DAGS
SUB R4 R0 ; DAGS - GADS
MOV R3 R0
TRP #1
; print DAGS
MOVI R3 ' '
TRP #3
; space
MOVI R3 '-'
TRP #3
; -
MOVI R3 ' '
TRP #3
; space
MOV R3 R1
TRP #1
; GADS
MOVI R3 ' '
TRP #3
; space
MOVI R3 '='
TRP #3
; =
MOVI R3 ' '
TRP #3
; space
MOV R3 R4
TRP #1
; result
MOVI R3 '\n'
TRP #3

TRP #0