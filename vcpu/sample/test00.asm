JMP 2
LOAD A,1
PUSH A
POP A
STORE A,#(800)
LOAD A,0
PUSH A
LOAD A,#(800)
PUSH A
POP B
POP A
MINUS
PUSH C
POP A
STORE A,#(800)
END
