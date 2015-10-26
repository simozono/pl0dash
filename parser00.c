#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokentable.h"

extern FILE *yyin;
extern int yylex();
extern char *yytext;

int getToken(void);
void parse_Expression(void);
void parse_Expression_dash(void);
void parse_Term(void);
void parse_Term_dash(void);
void parse_Factor(void);

int nextToken; /* 次のトークンが入る変数 */

int getToken(void) { /* トークンを取得する関数 */
  int token = yylex();
  if (token == 0) {token = T_EOF; } /* yylex()が0を返す時がEOFのようだ */
  return token;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf ("argument error\n");
    exit(0);
  }

  yyin = fopen(argv[1], "r");
  if (yyin  == NULL) {
    printf ("%s file not found.\n", argv[1]);
    exit(0);
  }

  nextToken = getToken();

  /* 構文解析スタート S=E */
  parse_Expression();
  if (nextToken != T_EOF) {
    printf ("parse error EOF\n");
    exit(0);
  }
}

void parse_Expression() { /* E → TE' */
  parse_Term();
  parse_Expression_dash();
}

void parse_Expression_dash() { /* E' → +TE'|ε */
  if (nextToken == T_PLUS) {
    nextToken = getToken();
    parse_Term();
    printf("process +\n");
    parse_Expression_dash();
  }
}

void parse_Term() {
  parse_Factor();
  parse_Term_dash();
}

void parse_Term_dash() {
  if (nextToken == T_MULTI) {
    nextToken = getToken();
    parse_Factor();
    printf("process *\n");
    parse_Term_dash();
  }
}

void parse_Factor() {
  if (nextToken == T_LPAR) {
    nextToken = getToken();
    parse_Expression();
    if (nextToken != T_RPAR) {
      printf ("error right paren\n");
      exit(0);
    }
    nextToken = getToken();
  } else if (nextToken == T_NUMBER) {
    printf ("process number %s\n", yytext);
    nextToken = getToken();
  } else {
    printf ("error token\n");
    exit(0);
  }
}
