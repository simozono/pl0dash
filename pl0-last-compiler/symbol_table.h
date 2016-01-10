#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_ID_NAME 31  /* ID の最大値 */

typedef enum { /* 記号表に登録する名前の種別 */
  const_id,  /* 定数 */
  var_id,    /* 変数 */
  func_id,   /* 関数 */
  param_id   /* 関数の仮引数 */
} Type_Id;

/* 記号表への登録関係関数 */
/* 返り値は全て記号表上の番号 */
extern int register_const_in_tbl(char *id, int value, int line_no); /* 定数 */
extern int register_var_in_tbl(char *id, int line_no);   /* 変数 */
extern int register_func_in_tbl(char *id, int line_no);  /* 関数 */
extern int register_param_in_tbl(char *id, int line_no); /* 仮引数 */

/* 記号表上に登録されたものの検索及び情報取得関係 */
/* ptr は記号表上の番号 */
extern int search_tbl(char *id);            /* idを検索する */
extern Type_Id get_symbol_type(int ptr);    /* 種別を返す */
extern char *get_symbol_name(int ptr);      /* IDの名前を返す */
extern int get_symbol_def_line_no(int ptr); /* IDが定義された行番号を返す */
extern int get_func_args(int ptr);      /* ID(関数を仮定)の仮引数の個数を返す */
extern int get_symbol_address(int ptr); /* IDが使用するメモリ上のアドレスを返す */

/* 関数対応処理 */
extern void blk_level_up();   /* ブロックレベルを上げる */
extern void blk_level_down(); /* ブロックレベルを下げる */
extern int end_param();

