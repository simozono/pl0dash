/* codegen.c */
#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "symbol_table.h"

int gen_code_no_arg(Operator o); /* 引数なし */
int gen_code_arg_ST(Operator o, int ptr); /* 引数として名前表上の番号 */
int gen_code_arg_v(Operator o, int value); /* 引数として値そのもの */
int gen_code_arg_v_ST(Operator o, int value, int ptr); /* 引数として値そのもの */

int gen_code_no_arg(Operator o) {
  if (o == wrl) {
    printf("PRINTLN\n");
    return 0;
  }

  if (o == end) {
    printf("END\n");
    return 0;
  }

  if (o == wrt) {
    printf("POP A\n");
    printf("PRINT A\n");
    return 0;
  }

  if (o == odd) {
    printf("POP A\n");
  } else { /* スタックから2つPOPするもの */
    printf("POP B\n");
    printf("POP A\n");
  }
  switch (o) {
  case pls:
    printf("PLUS\n");
    break;
  case min:
    printf("MINUS\n");
    break;
  case mul:
    printf("MULTI\n");
    break;
  case divi:
    printf("DIV\n");
    break;
  case odd:
    printf("CMPODD\n");
    break;
  case eq:
    printf("CMPEQ\n");
    break;
  case neq:
    printf("CMPNOTEQ\n");
    break;
  case lt:
    printf("CMPLT\n");
    break;
  case gt:
    printf("CMPGT\n");
    break;
  case le:
    printf("CMPLE\n");
    break;
  case ge:
    printf("CMPGE\n");
    break;
  default:
    break;
  }
  printf("PUSH C\n");
  return 0;
}

int gen_code_arg_V_ST(Operator o, int value, int ptr) {
  switch(o) {
  case str:
    printf("LOAD A,%d\n", value);
    printf("STORE A,#(%d)\n", get_symbol_address(ptr));
  default:
    break;
  }
  return 0;
}

int gen_code_arg_V(Operator o, int value) {
  switch(o) {
  case lod:
    printf("LOAD A,%d\n", value);
    printf("PUSH A\n");
  default:
    break;
  }
  return 0;
}


int gen_code_arg_ST(Operator o, int ptr) {
  switch(o) {
  case str:
    printf("POP A\n");
    printf("STORE A,#(%d)\n", get_symbol_address(ptr));
    break;
  case lod:
    printf("LOAD A,#(%d)\n", get_symbol_address(ptr));
    printf("PUSH A\n");
    break;
  default:
    break;
  }
  return 0;
}
