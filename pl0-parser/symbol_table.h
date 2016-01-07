#define MAX_ID_NAME 31  /* ID の最大値 */

typedef enum {
  const_id,
  var_id,
  func_id,
  param_id
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
