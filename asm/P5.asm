;Project 5 Fib

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
END TRP #0