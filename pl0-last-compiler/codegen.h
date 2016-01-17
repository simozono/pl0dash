/* codegen.h */

typedef enum {
  str, /* ストア */
  lod, /* ロード */
  pls, min, mul, divi, /* 四則演算 */
  odd, eq, neq, lt, gt, le, ge, /* 比較演算 */
  wrt, wrl, /* 表示関係 */
  enterf, leavef,  /* フレーム処理 */
  pushup, /* スタックポンタを動かす */
  jmp, jpc, call,  /* ジャンプ */
  ret, end /* 終了 */
} Opr;

extern int gencode_no_arg(Opr o);
extern int gencode_arg_ST(Opr o, int ptr); /* 引数として名前表上の番号 */
extern int gencode_arg_V(Opr o, int value); /* 引数として値そのもの */
extern int gencode_arg_V_ST(Opr o, int value, int ptr); 
extern int next_code();
extern void list_code(FILE *fp, int n_flag);
extern void backpatch(int code_lineno);

