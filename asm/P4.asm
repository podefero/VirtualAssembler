;Project 4 Factorial (Stack included)

; Data
n .INT #0 ; n = 0
PRINT_PROMPT .STR "Enter n (-1 Exits): 12 >= n >= -1\n"
PRINT_INVALID .STR "Invalid Entry: "

; Code
; set pointer then call function
MOV R14 FP ; PFP = FP
MOV FP SP ; FP = SP
MOV R15 PC ; Get Address
ADDI R15 #24
JMP MAIN ; call function
JMP END ; set return address here

FACTORIAL MOVI R3 #69
    TRP #1
    JMP END

MAIN PUSH R15 ; push return address
    PUSH R14 ; push pfp
    MAIN_DO LDA R3 PRINT_PROMPT ; load string
        TRP #5 ; print prompt
        TRP #2 ; get user input in R3
        STR R3 n ; set n = to user input
        ; compare n == -1
        ; if true return 0
        MOVI R2 #-1 ; to compare
        CMP R2 R3 ; n == -1. R2 should be 0 if true
        MOVI R13 #0 ; return value
        BRZ R2 END ; return if R2 = 0
        ; compare n > 12
        ; compare n < -1
        ; if n > 12 or n < -1 then
        ; print invalid enetry
        MOVI R0 #12 ; to compare
        MOV R2 R3 ; copy n
        CMP R2 R0 ; cmp n > 12 -> R2
        MOVI R1 #-1 ; to compare
        CMP R1 R3 ; cmp -1 > n -> R1
        OR R1 R2 ; if R1 or R2 was 1 set R1 to 1
        BGT R1 MAIN_SUB_INVALID ; if true jump
        ;else
        ;print the factorial
        TRP #1 ; print n
        MOVI R3 '!'
        TRP #3 ; print !
        MOVI R3 ' '
        TRP #3 ; print space
        MOVI R3 '='
        TRP #3 ; print =
        MOVI R3 ' '
        TRP #3 ; print space
        ; set up function call
        MOV R14 FP ; PFP = FP
        MOV SP FP ; FP = SP
        MOV R15 PC ; set return address
        ADDI R15 #24 ; offset
        JMP FACTORIAL
        TRP #1 ; print n, return here from FACTORIAL
        MAIN_SUB_INVALID LDA R3 PRINT_INVALID ; load string
            TRP #5 ; print invalid
            LDR R3 n ; load n
            TRP #1 ; print n
            MOVI R3 '.'
            TRP #3 ; print .
            MOVI R3 ' '
            TRP #3 ; print space
            JMP MAIN_DO ; go back to main_do





END TRP #0