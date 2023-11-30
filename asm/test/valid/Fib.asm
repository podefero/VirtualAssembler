;Test pushing activation records on statck
;int fib(int n)
;if(n<=1)
;return n
;return fib(n-1) + fib(n-2)
;Instructions
MAIN MOVI R0 #4 ; param n, count value
    ; set return address
    MOV R15 R16 ; store PC
    ADDI R15 #24
    JMP FUNC
    JMP END

;pfp = fp
;fp = sp
;sp += record
FUNC MOV R14 R20 ; pfp = fp
    ; push activation record on stack
PRE_BODY MOV R20 R19 ;fp = sp
    PUSH R15 ; push address
    PUSH R14 ; push PFP
    MOVI R1 #0 ; set to 0
    PUSH R1 ; return value
    PUSH R0 ; param n

    ; Function body
BODY MOVI R1 #1 ;use to compare
    ;if(n<=1)
    PEEK R0 ; get n
    CMP R1 R0 ; if n = 1 then R1 = 0, ; if n < 1 R1 = -1
    ;then return n
    BRZ R1 BASE_CASE
    BGT R1 BASE_CASE
    ;else fib(n-1) + fib(n-2)
    ;fib(n-1)
    ADDI R0 #-1 ; n -1
    MOV R15 R16 ; set return address before fib(n-2)
    ADDI R15 #24 ; add offset to pc
    JMP FUNC ; recure
;fib(n-2), return here
    PEEK R0 ; get n
    ADDI R0 #-1 ;F(n-2) set up param
    MOV R15 R16 ; set return address to RET
    ADDI R15 #24 ; offset
    JMP FUNC
    JMP RET

;set return value to n
BASE_CASE POP R0 ; pop n
    POP R1 ; pop return value
    PUSH R0 ; push n
    PUSH R0 ; pusn return value as n

; end of body function, return address at FP
; not popping yet, still need the record
BODY_END MOV R15 R20 ; get ptr of FP
    LDR R15 R15 ; dereference
    JMR R15 ; return

;pop two records, add the return values
;set third record to new return value
RET POP R5 ; toss Fn1
    POP R0 ; pop Fn1 return value
    POP R5 ; toss pfp
    POP R5 ; toss return address
    POP R5 ; tss Fn2
    POP R1 ; Fn2 return value
    POP R14 ; get pfp
    POP R5 ; toss return address
    ADD R0 R1 ; add return values
    POP R1 ; pop Fn3
    POP R5 ; toss Fn3 return value
    PUSH R0 ; push new return value
    PUSH R1 ; push Fn3
    MOV R20 R14 ; fp = pfp
    MOV R3 R0
    JMP BODY_END ;

END MOV R3 R0
TRP #1
TRP #0