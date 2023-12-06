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

FACTORIAL PUSH R15 ; push return address
    PUSH R14 ; push pfp
    PUSH R0 ; n
;IF BASE CASE
    MOVI R1 #1 ; value to compare
    PEEK R0 ; get n
    CMP R0 R1 ; cmp n, 1
    BRZ R0 FACTORIAL_BASE_CASE ; if n == 1 return 1
    MOVI R1 #0 ; to compare
    PEEK R0 ; get n
    CMP R0 R1 ; cmp n, 0
    BRZ R0 FACTORIAL_BASE_CASE ; if n== 0 return 1
    JMP FACTORIAL_RECURE
;THEN Return 1
    ;Pop record
    ;Push return value
    ;Jump to return address
    FACTORIAL_BASE_CASE MOVI R0 #1 ; set return value
    POP R1 ; get n
    POP R2 ; get PFP
    POP R3 ; get return address
    PUSH R0 ; push return value
    MOV FP R2 ; FP = PFP
    JMR R3
;ELSE RECURE
    ;Setup function call to recure
    FACTORIAL_RECURE MOV R14 FP ; PFP = FP
    MOV FP SP ; SP = FP
    PEEK R0 ; get n
    ADDI R0 #-1 ; decrement n
    MOV R15 PC ; set return addres to return
    ADDI R15 #24
    JMP FACTORIAL
;RETURN
    ;Assume we have a return value on stack (from base case)
    ;And subsequent return calls
    FACTORIAL_RETURN POP R0 ; get return value
    POP R1 ; get n
    POP R2 ; get PFP
    POP R3 ; get return address
    MUL R0 R1 ; calc new return value
    PUSH R0 ; push new return value on stack
    MOV FP R2 ; set FP = PFP
    JMR R3 ; return to address




MAIN PUSH R15 ; push return address
    PUSH R14 ; push pfp
    MAIN_DO LDA R3 PRINT_PROMPT ; load string
        TRP #5 ; print prompt
        TRP #2 ; get user input in R3
        STR R3 n ; set n = to user input
    ; if n == -1
        ; compare n == -1
        ; if true return 0
        MOVI R2 #-1 ; to compare
        CMP R2 R3 ; n == -1. R2 should be 0 if true
        BRZ R2 END ; return if R2 = 0
    ; if n > 12 or n < -1
        ; compare n > 12
        ; compare n < -1
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
        MOV FP SP ; FP = SP
        LDR R0 n ; pass param n
        MOV R15 PC ; set return address
        ADDI R15 #24 ; offset
        JMP FACTORIAL
        MOV R3 R0 ; get return value
        TRP #1 ; print n, return here from FACTORIAL
        MOVI R3 '\n'
        TRP #3 ; print new line
        TRP #3 ; print new line
        JMP MAIN_DO ; go back to main do
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