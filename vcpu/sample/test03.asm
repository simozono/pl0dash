LOAD A,3
STORE A,#(800)
JMP 4
LOAD A,9
PUSH A
POP A
STORE A,#(801)
LOAD A,#(801)
PUSH A
LOAD A,#(800)
PUSH A
POP B
POP A
CMPGT
PUSH C
POP C
JPC 26
LOAD A,1
PUSH A
POP A
STORE A,#(801)
LOAD A,3
PUSH A
POP A
PRINT A
LOAD A,#(801)
PUSH A
POP A
PRINT A
PRINTLN
END
