%{
  #include <stdio.h>
  extern char *yytext;
%}

%token T_ID T_NUMBER T_COLEQ T_EQ T_NOTEQ T_LT T_GT T_LE T_GE
%token T_PERIOD T_SEMIC T_COMMA T_LPAR T_RPAR T_PLUS T_MINUS
%token T_MULTI T_DIVIDE T_CONST T_VAR T_FUNC T_BEGIN T_END
%token T_IF T_THEN T_WHILE T_DO T_RETURN T_WRITE T_WRITELN
%token T_ODD 

%token T_WHITESPC T_ERROR T_EOF

%start expression

%%
expression:	expression T_PLUS term
			{ printf("POP B\n");
			  printf("POP A\n");
			  printf("PLUS\n");
			  printf("PUSH C\n");
			}
		| term
		;

term:		term T_MULTI factor
			{ printf("POP B\n");
			  printf("POP A\n");
			  printf("MULTI\n");
			  printf("PUSH C\n");
                        }
		| factor
		;

factor:		T_LPAR expression T_RPAR
		| T_NUMBER
			{ printf("LOAD A, %s\n", yytext);
			  printf("PUSH A\n");
                        }
		;
%%
int yyerror(char *error_message) {
  printf("ERROR %s\n", error_message);
  return 0;
}
