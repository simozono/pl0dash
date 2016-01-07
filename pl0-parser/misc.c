#include "misc.h"
#include "symbol_table.h"

void pl0_error(char *type, char *lexeme,  int line_no, char *error_message);
void info_ref_symbol(int ptr, int cur_line_no);

/* エラーを出す関数 */
void pl0_error(char *type, char *lexeme,  int line_no, char *error_message) { 
  fprintf(stderr, "%s エラー: %d 行目 (%s)付近: %s\n",
	  type, line_no, lexeme, error_message);
  exit(EXIT_FAILURE);
}

/* 使用されている名前がどこで定義されたものかの情報を出す */
void info_ref_symbol(int ptr, int cur_line_no) {
  char *id;
  int line_no;

  id = get_symbol_name(ptr);
  line_no = get_symbol_def_line_no(ptr);
  
  switch (get_symbol_type(ptr)) {
  case const_id:
    fprintf(stderr, "%d行目で使われている\"%s\"は%d行目で定義された[定数]です。\n",
	    cur_line_no, id, line_no);
    break;
  case var_id:
    fprintf(stderr, "%d行目で使われている\"%s\"は%d行目で定義された[変数]です。\n",
	    cur_line_no, id, line_no);
    break;
  case param_id:
    fprintf(stderr, "%d行目で使われている\"%s\"は%d行目で定義された[仮引数]です。\n",
	    cur_line_no, id, line_no);
    break;
  case func_id:
    fprintf(stderr, "%d行目で使われている\"%s\"は%d行目で定義された[関数]です。\n",
	    cur_line_no, id, line_no);
    break;
  }
}
