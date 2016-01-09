/* codegen.h */

typedef enum {
  pls, min, mul, divi, /* 四則演算 */
  odd, eq, neq, lt, gt, le, ge, /* 比較演算 */
  wrt, wrl, /* 表示関係 */
  jmp, jpc,  /* ジャンプ */
  end /* 終了 */
} Operator;

extern int gen_code_no_arg(Operator o);
