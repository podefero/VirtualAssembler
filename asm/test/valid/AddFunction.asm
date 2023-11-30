;Test pushing activation records on statck
;Will call function twice with different params
;Non-recursive test

;Instructions
MAIN MOVI R0 #5 ; param a
    MOVI R1 #3 ; param b
    ; set return address
    MOV R15 R16 ; store PC
    ADDI R15 #24
    JMP FUNC
    JMP END

;add function(int a, int b) returns a + b;
;pfp = fp
;fp = sp
;sp += record
FUNC MOV R14 R20 ; pfp = fp
    ; Prebody
    MOV R20 R19 ;fp = sp
    PUSH R15 ; push address
    PUSH R14 ; push PFP
    PUSH R0 ; param a
    PUSH R1 ; param b

    ; Function body (addition of a and b)
    POP R1        ; pop param b
    POP R0        ; pop param a
    ADD R0 R1  ; R0 = a + b
    MOV R3 R0 ; move result to R3 to print
    TRP #1 ; print result

    ; post body
    POP R14 ; pop pfp
    POP R15 ; pop return address
    MOV R20 R14 ; set fp = pfp
    JMR R15 ; return

END TRP #0