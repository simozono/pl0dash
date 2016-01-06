%{
  #include <stdio.h>
  #include <string.h>
  #ifdef YACC
  #include "tokentable_bison.h"
  #else
  #include "../lexer/tokentable.h"
  #endif
  int line_number = 1;
%}
%%
[[:blank:]]+		{}
\n			{ line_number += 1; }
"("			{ return T_LPAR;    }
")"			{ return T_RPAR;    }
"+"			{ return T_PLUS;    }
"-"			{ return T_MINUS;   }
"*"			{ return T_MULTI;   }
"/"			{ return T_DIVIDE;  }
[[:digit:]]+|"0x"[0-9a-h]+    { return T_NUMBER;   }
.                             { return T_ERROR; }
%%
