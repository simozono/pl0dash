LOAD A,10
STORE A,#(800)
JMP 4
LOAD A,1
PUSH A
POP A
STORE A,#(801)
LOAD A,#(801)
PUSH A
LOAD A,#(800)
PUSH A
POP B
POP A
CMPLE
PUSH C
POP C
JPC 40
LOAD A,#(801)
PUSH A
LOAD A,#(801)
PUSH A
POP B
POP A
MULTI
PUSH C
POP A
PRINT A
PRINTLN
LOAD A,#(801)
PUSH A
LOAD A,1
PUSH A
POP B
POP A
PLUS
PUSH C
POP A
STORE A,#(801)
JMP 8
LOAD A,1
PUSH A
POP A
PRINT A
PRINTLN
END