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

extern int register_const_in_tbl(char *id, int value, int line_no);
extern int register_var_in_tbl(char *id, int line_no);
extern int register_func_in_tbl(char *id, int line_no);
extern int register_param_in_tbl(char *id, int line_no);
extern int end_param();
extern int search_tbl(char *id);
extern Type_Id get_symbol_type(int ptr);
extern char *get_symbol_name(int ptr);
extern int get_func_args(int ptr);
extern int get_symbol_def_line_no(int ptr);
extern void blk_level_up();
extern void blk_level_down();
extern int get_symbol_address(int ptr);

