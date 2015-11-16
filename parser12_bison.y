/* PL/0' 用 (LA)LR構文解析器 No.01
 *              2015年後期 鹿児島高専
 *              3年生 言語処理系 授業用
 *   * 構文解析しか行っていない
 */

%{
  #include <stdio.h>
  #include <stdlib.h>
  extern char *yytext;
  extern int yylex(void);
  int yyerror(char *s);
%}

%token T_ID T_NUMBER T_COLEQ T_EQ T_NOTEQ T_LT T_GT T_LE T_GE
%token T_PERIOD T_SEMIC T_COMMA T_LPAR T_RPAR T_PLUS T_MINUS
%token T_MULTI T_DIVIDE T_CONST T_VAR T_FUNC T_BEGIN T_END
%token T_IF T_THEN T_WHILE T_DO T_RETURN T_WRITE T_WRITELN
%token T_ODD 

%token T_WHITESPC T_ERROR T_EOF

%start program

%%
program:        block T_PERIOD
                ;
block:          declList statement
                ;

declList:        /* ε */
        |       declList constDecl
        |       declList varDecl
        |       declList funcDecl
                ;

constDecl:      T_CONST constIdList T_SEMIC
                ;
constIdList:    constIdList T_COMMA T_ID T_EQ T_NUMBER
        |       T_ID T_EQ T_NUMBER
                ;

varDecl:        T_VAR varIdList T_SEMIC
                ;
varIdList:      varIdList T_COMMA T_ID
        |       T_ID
                ;

funcDecl:       T_FUNC T_ID T_LPAR funcDeclIdList T_RPAR block T_SEMIC
                ;
funcDeclIdList: /* ε */
        |       parameterList
                ;
parameterList:  parameterList T_SEMIC T_ID
        |       T_ID
                ;

statement:      /* ε */
        |       T_ID T_COLEQ expression
        |       T_BEGIN statementList  T_END {printf ("reduce to <statement>\n")}
        |       T_IF condition T_THEN statement
        |       T_WHILE condition T_DO statement
        |       T_RETURN expression
        |       T_WRITE expression
        |       T_WRITELN
                ;
statementList:  statementList T_SEMIC statement
        |       statement
                ;

condition:      T_ODD expression
        |       expression T_EQ    expression
        |       expression T_NOTEQ expression
        |       expression T_LT    expression
        |       expression T_GT    expression
        |       expression T_LE    expression
        |       expression T_GE    expression
                ;

expression:     term
        |       T_PLUS term
        |       T_MINUS term
        |       expression T_PLUS  term
        |       expression T_MINUS term
                ;

term:           factor
        |       term T_MULTI  factor
        |       term T_DIVIDE factor
                ;

factor:         T_ID
        |       T_NUMBER
        |       T_ID T_LPAR funcArgList T_RPAR
        |       T_LPAR expression T_RPAR
                ;
funcArgList:    /* ε */
        |       argExpList
                ;
argExpList:     argExpList T_COMMA expression
        |       expression
                ;

%%
int yyerror(char *error_message) {
  printf("ERROR %s\n", error_message);
  return(EXIT_FAILURE); /* 終了ステータスと同じものを返しておく */
}
