/* codegen.h */

typedef enum {
  str, /* ストア */
  lod, /* ロード */
  pls, min, mul, divi, /* 四則演算 */
  odd, eq, neq, lt, gt, le, ge, /* 比較演算 */
  wrt, wrl, /* 表示関係 */
  jmp, jpc,  /* ジャンプ */
  end /* 終了 */
} Operator;

extern int gen_code_no_arg(Operator o);
extern int gen_code_arg_ST(Operator o, int ptr); /* 引数として名前表上の番号 */
extern int gen_code_arg_V(Operator o, int value); /* 引数として値そのもの */
extern int gen_code_arg_V_ST(Operator o, int value, int ptr); 
