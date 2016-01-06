#ifndef TBL
#define TBL
#include "symbol_table.h"
#endif

#define MAXIDNAME 31 /* 名前(ID)の最大長 */

struct table_entry {
  Type_Id type;
  char name[MAXIDNAME];
  union {
    int value; /* 定数の場合、値 */
    struct {
      int n_params; /* 関数の場合、引数の個数 */
      int address;  /* 関数の場合、先頭アドレス */
    } f;
    int address; /* 定数、変数、引数の場合、アドレス */
  } u;
};

