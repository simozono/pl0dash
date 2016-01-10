#include "symbol_table.h"
#include "misc.h"

#define MAX_TABLE_LEN 200 /* 記号表の大きさ */
#define MAX_STACK_LEN  30 /* ptrスタックの大きさ(ブロックレベル) */

struct table_entry { /* 記号表に登録する要素 */
  char name[MAX_ID_NAME];  /* 名前 */
  Type_Id type;            /* 種別 */
  int line_no; /* 宣言されたソースコード上の行数 */
  union {
    int value; /* 定数の場合、値 */
    struct {
      int n_params; /* 関数の場合、仮引数の個数 */
      /* int address; 関数の場合、先頭アドレス */
    } f;
    /* int address; 定数、変数、仮引数の場合、アドレス */
  } u;
};

static struct table_entry symbol_table[MAX_TABLE_LEN]; /* 記号表 */
static int table_ptr = 0 ; /* 記号表の現在位置を示すポインタ */
                           /* symbol_table[0]は番兵で使用   */
static int cur_func_tptr;  /* 現在登録処理している関数の記号 */
			   /* 表上の位置を記憶(仮引数の処理) */
static int ptr_stack[MAX_STACK_LEN]; /* 「意味解析(記号表)」の回で    */
                                     /* 説明した元の ptr を覚えておく */
                                     /* スタック                     */
static int stack_ptr = 0; /* 上記スタック用 */

int register_const_in_tbl(char *id, int value, int line_no);
int register_var_in_tbl(char *id, int line_no);
int register_func_in_tbl(char *id, int line_no);
int register_param_in_tbl(char *id, int line_no);
int end_param();
int check_double_regist(char *id);
void rgst_name(char *id, int line_no);
int search_tbl(char *id);
Type_Id get_symbol_type(int ptr);
char *get_symbol_name(int ptr);
int get_symbol_def_line_no(int ptr);
int get_func_args(int ptr);
void blk_level_up();
void blk_level_down();


/* 名前の二重登録のチェック(登録されていたら1) */
int check_double_regist(char *id) {
  return 0;
}

/* 名前と行番号のみの登録(後述関数からのみ呼ばれる) */
void rgst_name(char *id, int line_no) {
  table_ptr++;
  if (table_ptr >= MAX_TABLE_LEN) {
    pl0_error("内部", id, line_no, "名前の登録数が多すぎ。");
  } else if (check_double_regist(id) == 1) {
    pl0_error("意味解析", id, line_no, "既に登録されている。");
  } else {
    strcpy(symbol_table[table_ptr].name, id);
  }
}

/* 記号表への定数の登録 */
/* id = 名前, value = 定数値, line_no = 行番号 */
int register_const_in_tbl(char *id, int value, int line_no) {
  rgst_name(id, line_no);
  symbol_table[table_ptr].type = const_id;
  symbol_table[table_ptr].line_no = line_no;
  symbol_table[table_ptr].u.value = value;
  return table_ptr;
}

/* 記号表への変数の登録 */
/* id = 名前, line_no = 行番号 */
int register_var_in_tbl(char *id, int line_no) {
  rgst_name(id, line_no);
  symbol_table[table_ptr].type = var_id;
  symbol_table[table_ptr].line_no = line_no;
  return table_ptr;
}

/* 関数関係登録処理 */
/* 記号表への関数の登録 */
/* id = 名前, line_no = 行番号 */
int register_func_in_tbl(char *id, int line_no) {
  rgst_name(id, line_no);
  symbol_table[table_ptr].type = func_id;
  symbol_table[table_ptr].line_no = line_no;
  symbol_table[table_ptr].u.f.n_params = 0; /* 仮引数の個数の初期値設定 */
  cur_func_tptr = table_ptr;   /* 現在登録処理している関数のptrを保存 */
  return table_ptr;
}


/* 記号表への仮引数の登録 */
/* id = 名前, line_no = 行番号 */
int register_param_in_tbl(char *id, int line_no) {
  rgst_name(id, line_no);
  symbol_table[table_ptr].type = param_id;
  symbol_table[table_ptr].line_no = line_no;
  symbol_table[cur_func_tptr].u.f.n_params++; /* 該当関数の仮引数の個数繰上 */
  return table_ptr;
}

/* ブロックレベル処理関係 */
/* ブロックレベルが一つ上がる */
void blk_level_up() {
  stack_ptr++;
  if (stack_ptr == MAX_STACK_LEN) pl0_error("内部", "", 0,
			       "blk_level_upの最大になった。");
  ptr_stack[stack_ptr] = table_ptr;
}

/* ブロックレベルが一つ下がる */
void blk_level_down() {
  if (stack_ptr < 1) pl0_error("内部", "", 0,
			       "blk_level_downがおかしい。");
  table_ptr = ptr_stack[stack_ptr];
  stack_ptr--;
}


/* 仮引数宣言部の最後で呼ばれる */
int end_param() {
    return 0;
}

/* 記号表検索 */
int search_tbl(char *id) {
  int i = table_ptr;
  strcpy(symbol_table[0].name, id); /* 番兵 */
  while(strcmp(id, symbol_table[i].name)) {
    i--;
  }
  return i; /* 名前がなかったら0, あったら1以上 */  
}

/* 記号表 ptr 位置の名前のタイプTypeId を返す */
Type_Id get_symbol_type(int ptr) {
  return symbol_table[ptr].type;
}

/* 記号表 ptr 位置の名前の定義された行数をを返す */
int get_symbol_def_line_no(int ptr) {
  return symbol_table[ptr].line_no;
}


/* 記号表 ptr 位置の名前を返す */
char *get_symbol_name(int ptr) {
  return symbol_table[ptr].name;
}


/* 記号表 ptr 位置の関数の引数の個数を返す */
int get_func_args(int ptr) {
  if (symbol_table[ptr].type != func_id) {
    pl0_error("内部", symbol_table[ptr].name, 0,
	      "get_func_argがおかしい");
  }
  return symbol_table[ptr].u.f.n_params;
}
