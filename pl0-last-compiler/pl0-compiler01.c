/* PL/0'(最終版)用 LL(1)再帰下降型コンパイラ01
 *      2015年後期 鹿児島高専
 *      3年生 言語処理系 授業用
 * * PL/0'(最終版)の文法に従っている
 * * 構文解析/意味解析/コード生成を行っている
 * * 関数のコード生成には未対応
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "../lexer/tokentable.h"
#include "symbol_table.h"
#include "../pl0-parser/misc.h"
#include "codegen.h"

extern FILE *yyin;      /* 読み込むソースファイル */
extern int yylex();     /* lex の字句解析 */
extern int line_no; /* 行番号 */
extern int t_num_value; /* T_NUMBER の実際の値 */
extern char *yytext;    /* lex よりレクシムが入る */

int getToken(void);

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
void parse_FuncBlock(void);
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
int parse_FuncArgList(void);
int parse_FuncArgList_dash(void);

int nextToken; /* 次のトークンが入る変数 */

int getToken(void) { /* トークンを取得する関数 */
  int token = yylex();
  if (token == 0) { /* yylex()が0を返す時がEOFのようだ */
    token = T_EOF;
  }
  return token;
}

int main(int argc, char *argv[]) {

  int opts; /* オプション処理 */
  int n_flag = 0; /* アセンブリ言語に番地をつけるかどうか */
  int o_flag = 0; /* 出力ファイルがあるかどうか */
  FILE *out_fp; /* 出力ファイル用ファイルポインタ */

  char outfilename[FILENAME_MAX]; /* 出力ファイル名 */


  while ((opts=getopt(argc, argv, "hno:")) != -1) {
    switch(opts) {
    case 'h':
      fprintf(stderr, "%s [-h] [-n] [-o output_file] source_file\n",
	      argv[0]);
      exit(EXIT_SUCCESS);
      break;
    case 'n':
      n_flag = 1;
      break;
    case 'o':
      o_flag = 1;
      strcpy(outfilename, optarg);
      break;
    case '?':
      exit(EXIT_FAILURE);
      break;
    }
  }

  if (optind >= argc) {
    fprintf (stderr, "ソースファイルを指定してください。\n");
    exit(EXIT_FAILURE);
  }

  yyin = fopen(argv[optind], "r");
  if (yyin  == NULL) {
    fprintf (stderr, "%s というファイルがありません。\n", argv[optind]);
    exit(EXIT_FAILURE);
  }

  out_fp = stdout;
  if (o_flag) {
    if ((out_fp = fopen(outfilename, "w")) == NULL) {
      fprintf(stderr, "%s という出力ファイルを作成できませんでした。\n",
	      outfilename);
      exit(EXIT_FAILURE);
    }
  }

  /* 構文解析スタート */
  nextToken = getToken();
  parse_Program();
  if (nextToken != T_EOF) pl0_error("構文", "EOF", line_no,
				    "ファイルの最後にきてしまいました。");
  gencode_no_arg(end);
  /* コード出力 */
  list_code(out_fp, n_flag);
}

void parse_Program() {
  parse_Block();
  if (nextToken != T_PERIOD) pl0_error("構文", yytext, line_no,
				       "ピリオドでない。");
  nextToken = getToken();
}

void parse_Block() { /* PL/0' 最終版の文法に対応 */
  parse_ConstDecl();
  parse_VarDecl();
  parse_FuncDecl();
  parse_Statement();
}

void parse_ConstDecl() {
  if (nextToken != T_CONST) return; /* T_CONST 以外は終了 */
  /* T_CONST では何もしない。次のトークンを読む */
  nextToken = getToken();
  parse_ConstIdList();
  if (nextToken != T_SEMIC) pl0_error("構文", yytext, line_no, ";がない。");
  nextToken = getToken();
  parse_ConstDecl();
}

void parse_ConstIdList() {
  int ptr;
  char cur_const_id[MAX_ID_NAME];
  if (nextToken != T_ID) pl0_error("構文", yytext, line_no, "定数名でない。");
  strcpy(cur_const_id, yytext);
  nextToken = getToken();
  if (nextToken != T_EQ) pl0_error("構文", yytext, line_no, "=がない。");
  nextToken = getToken();
  if (nextToken != T_NUMBER) pl0_error("構文", yytext, line_no, "数値でない。");
  /* 定数名の登録および値の設定 */
  ptr = register_const_in_tbl(cur_const_id, t_num_value, line_no);
  gencode_arg_V_ST(str, t_num_value, ptr); /* 定数用初期コード生成 */
  nextToken = getToken();
  parse_ConstIdList_dash();
}

void parse_ConstIdList_dash() {
  int ptr;
  char cur_const_id[MAX_ID_NAME];
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    if (nextToken != T_ID) pl0_error("構文", yytext, line_no, "定数名でない。");
    strcpy(cur_const_id, yytext);
    nextToken = getToken();
    if (nextToken != T_EQ) pl0_error("構文", yytext, line_no, "=がない。");
    nextToken = getToken();
    if (nextToken != T_NUMBER) pl0_error("構文", yytext, line_no,
					 "数値でない。");
    /* 定数名の登録および値の設定 */
    ptr = register_const_in_tbl(cur_const_id, t_num_value, line_no);
    gencode_arg_V_ST(str, t_num_value, ptr); /* 定数用初期コード生成 */
    nextToken = getToken();
    parse_ConstIdList_dash();
  }
}

void parse_VarDecl() {
  if (nextToken != T_VAR) return; /* T_VAR 以外は終了 */
  /* T_VAR では何もしない。次のトークンを読む */
  nextToken = getToken();
  parse_VarIdList();
  if (nextToken != T_SEMIC) pl0_error("構文", yytext, line_no, ";がない。");
  nextToken = getToken();
  parse_VarDecl();
}

void parse_VarIdList() {
  if (nextToken != T_ID) pl0_error("構文", yytext, line_no, "変数名でない。");
  /* 変数名の登録 */
  register_var_in_tbl(yytext, line_no);
  nextToken = getToken();
  parse_VarIdList_dash();
}

void parse_VarIdList_dash() {
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    if (nextToken != T_ID) pl0_error("構文", yytext, line_no, "変数名でない。");
    /* 変数名の記号表への登録 */
    register_var_in_tbl(yytext, line_no);
    nextToken = getToken();
    parse_VarIdList_dash();
  }
}

void parse_FuncBlock() { /* PL/0' 最終版の文法に対応 */
  parse_ConstDecl();
  parse_VarDecl();
  parse_Statement();
}

void parse_FuncDecl() {
  if (nextToken != T_FUNC) return; /* T_FUNC 以外は終了 */
  /* T_FUNC では何もしない。次のトークンを読む */
  nextToken = getToken();
  if (nextToken != T_ID) pl0_error("構文", yytext, line_no, "関数名でない。");
  /* 関数名の記号表への登録 */
  register_func_in_tbl(yytext, line_no);
  nextToken = getToken();
  if (nextToken != T_LPAR) pl0_error("構文", yytext, line_no, "(がない。");
  blk_level_up(); /* ブロックレベルを上げる */
  nextToken = getToken();
  parse_FuncDeclIdList();
  if (nextToken != T_RPAR) pl0_error("構文", yytext, line_no, ")がない。");
  nextToken = getToken();
  parse_FuncBlock();
  if (nextToken != T_SEMIC) pl0_error("構文", yytext, line_no, ";がない。");
  blk_level_down(); /* ブロックレベルを下げる */
  nextToken = getToken();
  parse_FuncDecl();
}

void parse_FuncDeclIdList() {
  if (nextToken == T_ID) {
    register_param_in_tbl(yytext, line_no); /* 仮引数を記号表へ登録 */
    nextToken = getToken();
    parse_FuncDeclIdList_dash();
  }
}

void parse_FuncDeclIdList_dash() {
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    if (nextToken != T_ID) pl0_error("構文", yytext, line_no,
				     "仮引数名でない。");
    register_param_in_tbl(yytext, line_no); /* 仮引数を記号表へ登録 */
    nextToken = getToken();
    parse_FuncDeclIdList_dash();
  }
}

void parse_Statement() {
  int ptr;  /* 現在の T_IDの記号表での位置  */
  int backpatch_if ; /* IF文用バックパッチ */
  int backpatch_while1 ; /* while文用バックパッチ1 */
  int backpatch_while2 ; /* while文用バックパッチ2 */
  if (nextToken == T_ID) { /* 代入文 */
    /* T_ID が変数/仮引数かの判断 */
    ptr = search_tbl(yytext); /* 現在の T_ID を検索 */
    if (ptr == 0) { /* なかった場合 */
      pl0_error("意味", yytext, line_no,
		"定義されていない名前を使用している。");
    } else if ((get_symbol_type(ptr) != var_id) &&
	       (get_symbol_type(ptr) != param_id)) { /* 変数/仮引数以外の場合 */
      pl0_error("意味", yytext, line_no, "変数/仮引数以外に代入している。");
    }
    nextToken = getToken();
    if (nextToken != T_COLEQ) pl0_error("構文", yytext, line_no, ":=がない。");
    nextToken = getToken();
    parse_Expression();
    /* 代入のコード生成 */
    gencode_arg_ST(str, ptr);
  } else if (nextToken == T_BEGIN) { /* begn ～ end */
    nextToken = getToken();
    parse_StatementList();
    if (nextToken != T_END) pl0_error("構文", yytext, line_no, "endがない。");
    nextToken = getToken();
  } else if (nextToken == T_IF) { /* if then */
    nextToken = getToken();
    parse_Condition();
    backpatch_if = gencode_arg_V(jpc, 0); /* 偽の場合の飛び先を仮に0とおく */
    if (nextToken != T_THEN) pl0_error("構文", yytext, line_no, "thenがない。");
    nextToken = getToken();
    parse_Statement();
    backpatch(backpatch_if);  /* jpc 0 をここで backpatch */
  } else if (nextToken == T_WHILE)  { /* while do */
    nextToken = getToken();
    backpatch_while1 = next_code(); /* while の条件先頭 */
    parse_Condition();
    if (nextToken != T_DO) pl0_error("構文", yytext, line_no,
				     "doがない。");
    backpatch_while2 = gencode_arg_V(jpc, 0); /* 偽の場合のとび先を仮に0とおく */
    nextToken = getToken();
    parse_Statement();
    gencode_arg_V(jmp, backpatch_while1); /* JMP の飛び先を条件先頭へ */
    backpatch(backpatch_while2); /* jpc 0 をここで backpatch */
  } else if (nextToken == T_RETURN) { /* return */
    nextToken = getToken();
    parse_Expression();
    /* ここで return のコード生成 */
  } else if (nextToken == T_WRITE) { /* write */
    nextToken = getToken();
    parse_Expression();
    /* write のコード生成 */
    gencode_no_arg(wrt);
  } else if (nextToken ==  T_WRITELN) { /* writeln */
    /* writeln のコード生成 */
    gencode_no_arg(wrl);
    nextToken = getToken();
  } else {
  }
}

void parse_StatementList() {
  parse_Statement();
  parse_StatementList_dash();
}

void parse_StatementList_dash() {
  if (nextToken == T_SEMIC) {
    nextToken = getToken();
    parse_Statement();
    parse_StatementList_dash();
  }
}

void parse_Condition() {
  int operator ; /* T_EQ や T_GT を一時格納 */
  if (nextToken == T_ODD) {
    nextToken = getToken();
    parse_Expression();
    /* ここで T_ODDの処理 */
    gencode_no_arg(odd);
  } else {
    parse_Expression();
    if (nextToken == T_EQ || nextToken == T_NOTEQ
        || nextToken == T_LT || nextToken == T_GT
        || nextToken == T_LE || nextToken == T_GE) {
      operator = nextToken;
    } else {
      pl0_error("構文", yytext, line_no, "比較演算子がない。");
    }
    nextToken = getToken();
    parse_Expression();
    /* ここで operator のコード生成 */
    switch(operator) {
    case T_EQ:
      gencode_no_arg(eq);
      break;
    case T_NOTEQ:
      gencode_no_arg(neq);
      break;
    case T_LT:
      gencode_no_arg(lt);
      break;
    case T_GT:
      gencode_no_arg(gt);
      break;
    case T_LE:
      gencode_no_arg(le);
      break;
    case T_GE:
      gencode_no_arg(ge);
      break;
    default:
      break;
    }
  }
}

void parse_Expression() {
  if (nextToken == T_PLUS) {
    nextToken = getToken();
    gencode_arg_V(lod, 0);
    parse_Term();
    /* ここで 0 Term + のコード生成 */
    gencode_no_arg(pls);
    parse_Expression_dash();
  } else if (nextToken == T_MINUS) {
    nextToken = getToken();
    gencode_arg_V(lod, 0);
    parse_Term();
    /* ここで 0 Term - のコード生成 */
    parse_Expression_dash();
    gencode_no_arg(min);
  } else {
    parse_Term();
    parse_Expression_dash();
  }
}

void parse_Expression_dash() {
  if (nextToken == T_PLUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで + のコード生成 */
    gencode_no_arg(pls);
    parse_Expression_dash();
  } else if (nextToken == T_MINUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで - のコード生成 */
    gencode_no_arg(min);
    parse_Expression_dash();
  } else {
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
    /* ここで * の処理 */
    gencode_no_arg(mul);
    parse_Term_dash();
  } else if (nextToken == T_DIVIDE) {
    nextToken = getToken();
    parse_Factor();
    /* ここで / の処理 */
    gencode_no_arg(divi);
    parse_Term_dash();
  } else {
  }
}

void parse_Factor() {
  int ptr;     /* 現在の T_IDの記号表での位置  */
  int f_args;  /* 関数の引数の個数 */
  if (nextToken == T_ID) { 
    /* T_ID が関数/変数/仮引数/定数かの判断 */
    ptr = search_tbl(yytext); /* 現在の T_ID を検索 */
    if (ptr == 0) { /* なかった場合 */
      pl0_error("意味", yytext, line_no,
		"定義されていない。");
    } else if (get_symbol_type(ptr) == func_id) { /* 関数の場合 */
      nextToken = getToken();
      if (nextToken != T_LPAR) pl0_error("構文", yytext, line_no, "(がない。");
      else {
        nextToken = getToken();
	f_args = parse_FuncArgList();
        if (nextToken != T_RPAR) pl0_error("構文", yytext, line_no,
					   ")がない。");
	/* 関数のチェック */
	info_ref_symbol(ptr, line_no); /* 情報出力 */
	if (get_func_args(ptr) != f_args)
	  pl0_error("意味", get_symbol_name(ptr), line_no, "引数の個数が違う");
      }
    } else {
      /* 関数以外(定数/変数/仮引数)の場合のコード生成 */
      gencode_arg_ST(lod, ptr);
    }
    nextToken = getToken();
  } else if (nextToken == T_NUMBER) {
    /* 数値の場合のコード生成 */
    gencode_arg_V(lod, t_num_value);
    nextToken = getToken();
  } else if (nextToken == T_LPAR) {
    nextToken = getToken();
    parse_Expression();
    if (nextToken != T_RPAR) pl0_error("構文", yytext, line_no, ")がない。");
    nextToken = getToken();
  } else {
    pl0_error("構文", yytext, line_no, "数値/変数/定数/関数でない。");
  }
}

int parse_FuncArgList() { /* 返り値は引数の個数 */
  int n_args = 0;
  if (nextToken == T_PLUS || nextToken == T_MINUS
      || nextToken == T_ID || nextToken == T_NUMBER
      || nextToken == T_LPAR) { /* First(<Expression>)に含まれるもの */
    parse_Expression();
    n_args = 1 + parse_FuncArgList_dash();
  } else {
  }
  return n_args;
}

int parse_FuncArgList_dash() { /* 返り値は引数の個数 */
  int n_args = 0;
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    parse_Expression();
    n_args = 1 + parse_FuncArgList_dash();
  } else {
  }
  return n_args;
}
