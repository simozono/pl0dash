#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokentable.h"

extern FILE *yyin;
extern int yylex();
extern char *yytext;

int getToken(void); /* トークンを取得する関数 */
void parse_Expression(void);
void parse_Expression_dash(void);
void parse_Term(void);
void parse_Term_dash(void);
void parse_Factor(void);
void parse_error(char *error_message); /* エラーメッセージを出す */

int nextToken; /* 次のトークンが入る変数 */

int getToken(void) {
  int token = yylex();
  if (token == 0) {token = T_EOF; } /* yylex()が0を返す時がEOFのようだ */
  return token;
}

void parse_error(char *error_message) {
  printf ("parse error: %s\n", error_message);
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf ("argument error\n");
    exit(0);
  }

  yyin = fopen(argv[1], "r"); /* ファイルを開く処理 */
  if (yyin  == NULL) {
    printf ("%s file not found.\n", argv[1]);
    exit(0);
  }

  /* 最初に1トークン読み込んでおく */
  nextToken = getToken(); 

  /* 構文解析スタート S=E */
  parse_Expression();
  if (nextToken != T_EOF) parse_error("not EOF");
  printf("END\n");
}

void parse_Expression() { /* E → TE' */
  parse_Term();
  parse_Expression_dash();
}

void parse_Expression_dash() { /* E' → +TE'|ε */
  if (nextToken == T_PLUS) {
    nextToken = getToken();
    parse_Term();
    /* T_PLUS の処理はじまり */
    printf("POP B\n");
    printf("POP A\n");
    printf("PLUS\n");
    printf("PUSH C\n");
    /* T_PLUS の処理おわり */
    parse_Expression_dash();
  }
}

void parse_Term() { /* T → FT' */
  parse_Factor();
  parse_Term_dash();
}

void parse_Term_dash() { /* T' → *FT'|ε */
  if (nextToken == T_MULTI) {
    nextToken = getToken();
    parse_Factor();
    /* T_MULTI の処理はじまり */
    printf("POP B\n");
    printf("POP A\n");
    printf("MULTI\n");
    printf("PUSH C\n");
    /* T_MULTI の処理おわり */
    parse_Term_dash();
  }
}

void parse_Factor() { /* F → (E)|T_NUMBER */
  if (nextToken == T_LPAR) {
    nextToken = getToken();
    parse_Expression();
    if (nextToken != T_RPAR) parse_error("not )");
    nextToken = getToken();
  } else if (nextToken == T_NUMBER) {
    /* T_NUMBER の処理はじまり */
    printf("LOAD A %s\n", yytext);
    printf("PUSH A\n");
    /* T_NUMBER の処理おわり */
    nextToken = getToken();
  } else {
    parse_error("not number and not (");
  }
}
