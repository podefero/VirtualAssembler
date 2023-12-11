;Project 5 Fib
;Strings
COMMA_SPACE .STR ", "
INPUT_LINE .STR "Fibonacci of "
IS .STR " is "
;Instructions
JMP MAIN ; go to main


FIB MOV R14 FP ; get fp
    ADDI R14 #-8 ; ptr n
    LDR R1 R14 ; get n
;IF BASE CASE (n<=1)
    CMPI R1 #1 ; 0 or -1 branch to base case
    BLT R1 BASE_CASE
    BRZ R1 BASE_CASE
    JMP FIB_1
;THEN
BASE_CASE POP R1 ; get n
    POP R14 ; get pfp
    POP R15 ; get return address
    PUSH R1 ; push return val (n)
    MOV FP R14 ; FP = PFP
    JMR R15 ; jump to return

;FIB_1 fib(n-1)
FIB_1 PEEK R1
    ADDI R1 #-1 ; decrement n by 1
    MOV R14 FP ; pfp
    MOV FP SP ; FP = SP
    MOV R15 PC ; setup return address
    ADDI R15 #60 ;offset address
    PUSH R15 ; push return address
    PUSH R14 ; push pfp
    PUSH R1 ; push n-1
    JMP FIB ; Recure
;Return here
;FIB_2 fib(n-2)
    MOV R14 FP ; get fp
    ADDI R14 #-8 ; ptr n
    LDR R1 R14 ; get n
    ADDI R1 #-2 ; decrement n by 2
    MOV R14 FP ; pfp
    MOV FP SP ; FP = SP
    MOV R15 PC ; setup return address
    ADDI R15 #60 ;offset address
    PUSH R15 ; push return address
    PUSH R14 ; push pfp
    PUSH R1 ; push n-1
    JMP FIB ; Recure
;RETURN
    POP R0 ; get first return val
    POP R1 ; get second return val
    ADD R0 R1 ; add and get new return val
    POP R1 ; pop n
    POP R14 ; pop PFP
    POP R15 ; pop return address
    MOV FP R14 ; FP = PFP
    PUSH R0 ; push new return val
    JMR R15 ; jump to return

MAIN PUSH R15 ; push return address 0
    PUSH R14 ; push pfp -4
    MOVI R1 #30 ; array size
    PUSH R1 ; push array size -8
    MOVI R1 #0 ; i
    PUSH R1 ; push i -12
    MOV R1 FP ; get FP
    ADDI R1 #-20 ; get offset from bot of stack
    ;point at start of array
    PUSH R1 ; push array ptr. -16
    ADDI R1 #-120 ; allocate array (30 * 4)
    MOV SP R1 ; set new sp
    MOVI R1 #0 ; n=0, fibval=0
    PUSH R1 ; push n -140
    PUSH R1 ; push fibval -144
;WHLE LOOP
;Get and handle input
MAIN_WHILE TRP #2 ; read input in R3
    MOV R14 FP ; get fp
    ADDI R14 #-140 ; point at n
 ;cin<<n
    STR R3 R14 ; set n
;CMP (n==-1) result in R1
    MOVI R1 #-1 ; use to compare R3
    CMP R1 R3 ; if -1 == n, R1 = 0
    ADDI R1 #1 ; if 0 make 1 (true)
;CMP (i>=ARRsize) result in R4
    MOV R14 FP ; get fp
    ADDI R14 #-8 ; point at ARRsize
    LDR R3 R14 ; get ARRsize
    ADDI R14 #-4 ; point at i
    LDR R4 R14 ; get i
    CMP R4 R3 ; if i>=ARRsize, R4 = 1, if = then 0
    CMPI R4 #-1 ; if it's 1 or 0 it'll be 1. else 0.
;(n==-1 || i>= ARRsize)
    OR R1 R4 ; If false loop else break
    BGT R1 MAIN_FOR ; break
;CALL FIB
;Set values
    ;set up call
    MOV R14 FP ; get fp
    ADDI R14 #-140 ; get n ptr
    LDR R1 R14 ; get n
    MOV R14 FP ; pfp = fp
    MOV FP SP ; fp = sp
; set up return address
    MOV R15 PC ; return address
    ADDI R15 #60 ; offset return address
; make record
    PUSH R15 ; push return address
    PUSH R14 ; push pfp
    PUSH R1 ; push n
    JMP FIB ; jump to function
;Return here from FIB
    POP R3 ; pop return val
    MOV R14 FP ; get fp
    ADDI R14 #-144 ; ptr fibval
;fibval=fib(n)
    STR R3 R14
    MOV R10 R3 ; save fibval for later
    MOV R14 FP ; i ptr
    MOV R13 FP ; arr ptr
    MOV R12 FP ; n ptr
    ADDI R12 #-140 ; offste to ptr n
;cout<< "Fibonacci of " << n << " is " << fibVal << endl;    LDA R3 INPUT_LINE
    LDA R3 INPUT_LINE
    TRP #5 ; print line
    LDR R3 R12 ; get n
    TRP #1 ; print n
    LDA R3 IS
    TRP #5 ; print line
    MOV R3 R10 ; get fibval
    TRP #1 ; print fibval
    MOVI R3 '\n' ; new line
    TRP #3 ; print new line
    ADDI R14 #-12 ; offset ptr to i
    LDR R0 R14 ; get i
    ADDI R13 #-16 ; offset ptr to array ptr
    LDR R1 R13 ; get array ptr
    MOVI R2 #-4 ; get 4
    MUL R0 R2 ; i * 4. get array offset
    ADD R1 R0 ; array ptr + offset
    LDR R4 R12 ; get n
;ARR[i] = n
    STR R4 R1 ; store n in array
    LDR R0 R14 ; get i
    ADDI R0 #1 ; i + 1
; i++
    STR R0 R14 ; store i
    ADDI R1 #-4 ; move array index to next item
;ARR[i] = fibVal
    STR R10 R1 ; store fibVal
    ADDI R0 #1 ;
    STR R0 R14 ; update i in stack
    MOV R12 FP ; get fp
    ADDI R12 #-8 ; ptr ARRsize
    LDR R2 R12 ; get ARRsize
    CMP R0 R2 ; cmp i, ARRsize
;if(i>=ARRsize) break
    BGT R0 MAIN_FOR ; break
    BRZ R0 MAIN_FOR ; break
    JMP MAIN_WHILE ; loop

MAIN_FOR MOVI R0 #0 ; j
    LDR R5 R14 ; get i
MAIN_LOOP MOVI R2 #2 ; get 2
   LDR R1 R14 ; get i
   DIV R1 R2 ; i / 2, use to compare
   MOV R2 R0 ; move j to R2
;j < i/2
   CMP R2 R1 ; j < i/2
   BRZ R2 END ;
   BGT R2 END ;
;cout<< ARR[j] << ", " << ARR[i-j-1]
    MOVI R2 #-4 ; get 4
    MUL R2 R0 ; 4 * j, get offset
    LDR R4 R13 ; array ptr
    ADD R2 R4 ; offset + array ptr
;ARR[j]
    LDR R3 R2 ; get ARR[j]
    TRP #1 ; print value
    LDA R3 COMMA_SPACE
    TRP #5 ; print comma space
    MOV R4 R5 ; move i
    SUB R4 R0 ; i - j
    ADDI R4 #-1 ; i - j -1
    MOVI R2 #-4 ; get 4
    MUL R2 R4 ; 4 * R2 get offset
    LDR R4 R13 ; get array ptr
    ADD R2 R4 ; offset + array ptr
;ARR[i-j-1]
    LDR R3 R2 ; get ARR[i-j-1]
    TRP #1 ; print value
;if(j+1 < i/2)
    MOV R4 R0 ; move j to R4
    ADDI R4 #1 ; j + 1
    CMP R4 R1 ; cmp(j+1 < i/2)
; if not true then continue
    BGT R4 CONTINUE
    BRZ R4 CONTINUE
; print
    LDA R3 COMMA_SPACE
    TRP #5 ; print
    ADDI R0 #1 ;j + 1
    JMP MAIN_LOOP
;j++
CONTINUE ADDI R0 #1 ; j + 1
    MOVI R3 '\n'
    TRP #3
    JMP MAIN_LOOP
;EXIT PROGRAM
END TRP #0