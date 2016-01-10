/* PL/0' 用 LL(1)再帰下降型構文解析器 No.02
 *              2015年後期 鹿児島高専
 *              3年生 言語処理系 授業用
 *   * 構文解析しか行っていない
 *   * ループを使わず再帰のみでやっている
 *   * 変数のIDと関数のIDの分岐をしているバージョン
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/tokentable.h"

extern FILE *yyin;      /* 読み込むソースファイル */
extern int yylex();     /* lex の字句解析 */
extern int line_no; /* 行番号 */
extern char *yytext;    /* lex よりレクシムが入る */

int checkTable(char *funcName);
int addTable(char *funcName);

int getToken(void);
void pl0parse_error(char *s);

/* 非終端記号に対応した関数 */
void parse_Program(void);
void parse_Block(void);
void parse_Decl(void);
void parse_ConstDecl(void);
void parse_ConstIdList(void);
void parse_ConstIdList_dash(void);
void parse_VarDecl(void);
void parse_VarIdList(void);
void parse_VarIdList_dash(void);
void parse_FuncDecl(void);
void parse_FuncDeclIdList(void);
void parse_FuncDeclIdList_dash(void);
void parse_Statement(void);
void parse_StatementList(void);
void parse_StatementList_dash(void);
void parse_Condition(void);
void parse_Expression(void);
void parse_Expression_dash(void);
void parse_Term(void);
void parse_Term_dash(void);
void parse_Factor(void);
void parse_FuncArgList(void);
void parse_FuncArgList_dash(void);

int nextToken; /* 次のトークンが入る変数 */

/*
 *  関数名テーブルの実装
 *
 */
#define  TABLEMAX  256
char funcTable[TABLEMAX][64];
int  tableIndex = 0;

int checkTable(char *funcName) {
  /* 指定した名前の関数があるか調べる */
  int flag = 0, i;
  for (i = 0; i < tableIndex; i++) {
    if (strcmp(funcName, funcTable[i]) == 0) flag = 1;
  }
  return flag; /* 同じ名前の関数があったら1を返す  */
}

int addTable(char *funcName) {
  /* 関数名が被らないか調べる */
  if (checkTable(funcName)) return -1; /* 関数名がかぶったら-1を返す */
  if (tableIndex == TABLEMAX) return -1; /* これ以上関数名テーブルに追加できない */

  /* 関数名をテーブルに追加 */
  strcpy(funcTable[tableIndex], funcName);
  tableIndex++;
  return 0;
}
/* 関数名テーブル関係 ここまで */

int getToken(void) { /* トークンを取得する関数 */
  int token = yylex();
  if (token == 0) { /* yylex()が0を返す時がEOFのようだ */
    token = T_EOF;
  }
  return token;
}

void pl0parse_error(char *error_message) { /* 構文エラーを出す関数 */
  printf("parse error near line %d(%s): %s\n",
         line_no, yytext, error_message);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf ("argument error\n");
    exit(EXIT_FAILURE);
  }

  yyin = fopen(argv[1], "r");
  if (yyin  == NULL) {
    printf ("%s file not found.\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  /* 構文解析スタート */
  nextToken = getToken();
  parse_Program();
  if (nextToken != T_EOF) pl0parse_error("not EOF");
}

void parse_Program() {
  printf("Enter Program\n");
  parse_Block();
  if (nextToken != T_PERIOD) pl0parse_error("not period");
  nextToken = getToken();
}

void parse_Block() {
  printf("Enter Block\n");
  parse_Decl();
  parse_Statement();
}

void parse_Decl() {
  printf("Enter Decl\n");
  if (nextToken == T_CONST) {
    parse_ConstDecl();
    parse_Decl();
  } else if (nextToken == T_VAR) {
    parse_VarDecl();
    parse_Decl();
  } else if (nextToken == T_FUNC) {
    parse_FuncDecl();
    parse_Decl();
  } else {
  }
}

void parse_ConstDecl() {
  printf("Enter ConstDecl\n");
  /* T_CONST では何もしない。次のトークンを読む */
  nextToken = getToken();
  parse_ConstIdList();
  if (nextToken != T_SEMIC) pl0parse_error("not ;");
  nextToken = getToken();
}

void parse_ConstIdList() {
  printf("Enter ConstIdList\n");
  if (nextToken != T_ID) pl0parse_error("not ID");
  nextToken = getToken();
  if (nextToken != T_EQ) pl0parse_error("not =");
  nextToken = getToken();
  if (nextToken != T_NUMBER) pl0parse_error("not number");
  /* 定数名の登録および値の設定をここで行う */
  nextToken = getToken();
  parse_ConstIdList_dash();
}

void parse_ConstIdList_dash() {
  printf("Enter ConstIdList_dash\n");
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    if (nextToken != T_ID) pl0parse_error("not ID");
    nextToken = getToken();
    if (nextToken != T_EQ) pl0parse_error("not =");
    nextToken = getToken();
    if (nextToken != T_NUMBER) pl0parse_error("not number");
    /* 定数名の登録および値の設定をここで行う */
    nextToken = getToken();
    parse_ConstIdList_dash();
  }
}

void parse_VarDecl() {
  printf("Enter VarDecl\n");
  /* T_VAR では何もしない。次のトークンを読む */
  nextToken = getToken();
  parse_VarIdList();
  if (nextToken != T_SEMIC) pl0parse_error("not semic");
  nextToken = getToken();
}

void parse_VarIdList() {
  printf("Enter VarIdList\n");
  if (nextToken != T_ID) pl0parse_error("not ID");
  /* 変数名の登録をここで行う */
  nextToken = getToken();
  parse_VarIdList_dash();
}

void parse_VarIdList_dash() {
  printf("Enter VarIdList_dash\n");
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    if (nextToken != T_ID) pl0parse_error("not ID");
    /* 変数名の登録をここで行う */
    nextToken = getToken();
    parse_VarIdList_dash();
  }
}

void parse_FuncDecl() {
  printf("Enter FuncDecl\n");
  /* T_FUNC では何もしない。次のトークンを読む */
  nextToken = getToken();
  if (nextToken != T_ID) pl0parse_error("not ID");

  /* 関数名の関数名テーブルへの追加 */
  if (addTable(yytext) == -1) {
    pl0parse_error("already defined ID");
  } else {
    printf("Add FuncNameTable\n");
  }
  
  nextToken = getToken();
  if (nextToken != T_LPAR) pl0parse_error("not (");
  nextToken = getToken();
  parse_FuncDeclIdList();
  if (nextToken != T_RPAR) pl0parse_error("not )");
  nextToken = getToken();
  parse_Block();
  if (nextToken != T_SEMIC) pl0parse_error("not semic");
  nextToken = getToken();
}

void parse_FuncDeclIdList() {
  printf("Enter FuncDeclIdList\n");
  if (nextToken == T_ID) {
    nextToken = getToken();
    parse_FuncDeclIdList_dash();
  }
}

void parse_FuncDeclIdList_dash() {
  printf("Enter FuncDeclIdList_dash\n");
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    if (nextToken != T_ID) pl0parse_error("not id)");
    nextToken = getToken();
    parse_FuncDeclIdList_dash();
  }
}

void parse_Statement() {
  printf("Enter Statement\n");
  if (nextToken == T_ID) { /* 代入文 */
    nextToken = getToken();
    if (nextToken != T_COLEQ) pl0parse_error("not :=");
    nextToken = getToken();
    parse_Expression();
  } else if (nextToken == T_BEGIN) { /* begn ～ end */
    nextToken = getToken();
    parse_StatementList();
    if (nextToken != T_END) pl0parse_error("not end");
    nextToken = getToken();
  } else if (nextToken == T_IF) { /* if then */
    nextToken = getToken();
    parse_Condition();
    if (nextToken != T_THEN) pl0parse_error("not then");
    nextToken = getToken();
    parse_Statement();
  } else if (nextToken == T_WHILE)  { /* while do */
    nextToken = getToken();
    parse_Condition();
    if (nextToken != T_DO) pl0parse_error("not do");
    nextToken = getToken();
    parse_Statement();
  } else if (nextToken == T_RETURN) { /* return */
    nextToken = getToken();
    parse_Expression();
  } else if (nextToken == T_WRITE) { /* write */
    nextToken = getToken();
    parse_Expression();
  } else if (nextToken ==  T_WRITELN) { /* writeln */
    /* writeln の処理 */
    nextToken = getToken();
  } else {
  }
}

void parse_StatementList() {
  printf("Enter StatementList\n");
  parse_Statement();
  parse_StatementList_dash();
}

void parse_StatementList_dash() {
  printf("Enter StatementList_dash\n");
  if (nextToken == T_SEMIC) {
    nextToken = getToken();
    parse_Statement();
    parse_StatementList_dash();
  }
}

void parse_Condition() {
  printf("Enter Condition\n");
  int operator ; /* T_EQ や T_GT を一時格納 */
  if (nextToken == T_ODD) {
    nextToken = getToken();
    parse_Expression();
    /* ここで T_ODDの処理 */
  } else {
    parse_Expression();
    if (nextToken == T_EQ || nextToken == T_NOTEQ
        || nextToken == T_LT || nextToken == T_GT
        || nextToken == T_LE || nextToken == T_GE) {
      operator = nextToken;
    } else {
      pl0parse_error("not condition operator");
    }
    nextToken = getToken();
    parse_Expression();
    /* ここで operator 処理 */
  }
}

void parse_Expression() {
  printf("Enter Expressions\n");
  if (nextToken == T_PLUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで 0 Term + をスタックにつむ */
    parse_Expression_dash();
  } else if (nextToken == T_MINUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで 0 Term - をスタックにつむ */
    parse_Expression_dash();
  } else {
    parse_Term();
    parse_Expression_dash();
  }
}

void parse_Expression_dash() {
  printf("Enter Expression_dash\n");
  if (nextToken == T_PLUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで + の処理 */
    parse_Expression_dash();
  } else if (nextToken == T_MINUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで - の処理 */
    parse_Expression_dash();
  } else {
  }
}

void parse_Term() {
  printf("Enter Term\n");
  parse_Factor();
  parse_Term_dash();
}

void parse_Term_dash() {
  printf("Enter Term_dash\n");
  if (nextToken == T_MULTI) {
    nextToken = getToken();
    parse_Factor();
    /* ここで * の処理 */
    parse_Term_dash();
  } else if (nextToken == T_DIVIDE) {
    nextToken = getToken();
    parse_Factor();
    /* ここで / の処理 */
    parse_Term_dash();
  } else {
  }
}

void parse_Factor() {
  printf("Enter Factor\n");
  if (nextToken == T_ID) { 
    /* 右辺値変数 or 関数呼び出しの判断をしなければならない */
    if (checkTable(yytext)) {  /* 関数名テーブルにあった場合の処理 */ 
      printf("Enter Func\n");
      nextToken = getToken();
      if (nextToken != T_LPAR) pl0parse_error("not (");
      else {
        nextToken = getToken();
        parse_FuncArgList();
        if (nextToken != T_RPAR) pl0parse_error("not )");
      }
    } else { /* 関数名テーブルになかったら変数/定数のはず */
    }
    nextToken = getToken();
  } else if (nextToken == T_NUMBER) { 
    /* ここで数字の処理 */
    nextToken = getToken();
  } else if (nextToken == T_LPAR) {
    nextToken = getToken();
    parse_Expression();
    if (nextToken != T_RPAR) pl0parse_error("not )");
    nextToken = getToken();
  } else {
    pl0parse_error("not factor");
  }
}

void parse_FuncArgList() {
  printf("Enter FuncArgList\n");
  if (nextToken == T_PLUS || nextToken == T_MINUS
      || nextToken == T_ID || nextToken == T_NUMBER
      || nextToken == T_LPAR) { /* First(<Expression>)に含まれるもの */
    parse_Expression();
    parse_FuncArgList_dash();
  } else {
  }
}

void parse_FuncArgList_dash() {
  printf("Enter FuncArgList_dash\n");
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    parse_Expression();
    parse_FuncArgList_dash();
  } else {
  }
}
