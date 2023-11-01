;Data Segment

;Hello
H .BYT 'H'
e .BYT 'e'
l .BYT 'l'
o .BYT 'o'

;My name
N .BYT 'N'
a .BYT 'a'
t .BYT 't'
h .BYT 'h' ; lower case h
n .BYT 'n'

s .BYT #32 ; space
NL .BYT 0x0a ; newline

; I can print ;
I .BYT 'I'
c .BYT 'c'
p .BYT 'p'
i .BYT 'i'
r .BYT 'r'
SEMI .BYT ';'

;numbers
ONE .INT #1
TWO .INT #2
THREE .INT #3
FIVE .INT #5a

;number ASCII
one .BYT '1'
two .BYT '2'
three .BYT '3'
five .BYT '5'

;operators
PLUS .BYT '+'
MULT .BYT '*'
MINUS .BYT '-'
DIVIDE .BYT '/'
EQUAL .BYT '='

;goodbye
G .BYT 'G'
d .BYT 'd'
b .BYT 'b'
y .BYT 'y'

;end of data seg
;instructions

;print Hello Nathan
LDB R3 H 
TRP #3
LDB R3 e
TRP #3
LDB R3 l
TRP #3
TRP #3
LDB R3 o
TRP #3
LDB R3 s
TRP #3
LDB R3 N
TRP #3
LDB R3 a
TRP #3
LDB R3 t
TRP #3
LDB R3 h
TRP #3 
LDB R3 a
TRP #3
LDB R3 n
TRP #3
LDB R3 NL
TRP #3

; print 1 + 3 = 4 (use add to print 4)
LDB R3 one
TRP #3
LDB R3 s
TRP #3 
LDB R3 PLUS
TRP #3
LDB R3 s
TRP #3 
LDB R3 three
TRP #3
LDB R3 s
TRP #3
LDB R3 EQUAL
TRP #3
LDB R3 s
TRP #3
LDR R3 ONE
LDR R4 THREE
ADD R3 R4
TRP #1
LDB R3 NL
TRP #3

; print 2 * 3 = 6
LDB R3 two
TRP #3
LDB R3 s
TRP #3
LDB R3 MULT
TRP #3
LDB R3 s
TRP #3
LDB R3 three
TRP #3
LDB R3 s
TRP #3
LDB R3 EQUAL
TRP #3
LDB R3 s
TRP #3
LDR R3 TWO
LDR R4 THREE
MUL R3, R4
TRP #1
LDB R3 NL
TRP #3

; print 1 - 5 = 4
LDB R3 one
TRP #3
LDB R3 s
TRP #3
LDB R3 MINUS
TRP #3
LDB R3 s
TRP #3
LDB R3 five
TRP #3
LDB R3 s
TRP #3
LDB R3 EQUAL
TRP #3
LDB R3 s
TRP #3
LDR R3 ONE
LDR R4 FIVE
SUB R3 R4
TRP #1
LDB R3 NL
TRP #3

; print 5 / 2 = 2
LDB R3 five
TRP #3
LDB R3 s
TRP #3
LDB R3 DIVIDE
TRP #3
LDB R3 s
TRP #3
LDB R3 two
TRP #3
LDB R3 s
TRP #3
LDB R3 EQUAL
TRP #3
LDB R3 s
TRP #3
LDR R3 FIVE
LDR R4 TWO
DIV R3 R4
TRP #1
LDB R3 NL
TRP #3

;print I can print ;
LDB R3 I
TRP #3
LDB R3 s
TRP #3
LDB R3 c
TRP #3
LDB R3 a
TRP #3
LDB R3 n
TRP #3
LDB R3 s
TRP #3
LDB R3 p
TRP #3
LDB R3 r
TRP #3
LDB R3 i
TRP #3
LDB R3 n
TRP #3
LDB R3 t
TRP #3
LDB R3 s
TRP #3
LDB R3 SEMI
TRP #3
LDB R3 NL
TRP #3

;print Goodbye
LDB R3 G
TRP #3
LDB R3 o
TRP #3
TRP #3
LDB R3 d
TRP #3
LDB R3 b
TRP #3
LDB R3 y
TRP #3
LDB R3 e
TRP #3
LDB R3 NL
TRP #3

;end program
TRP #0

