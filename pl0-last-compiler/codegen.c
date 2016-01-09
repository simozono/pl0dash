/* codegen.c */
#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"

int gen_code_no_arg(Operator o); /* 引数なし */

int gen_code_no_arg(Operator o) {

  if (o == wrl) {
    printf("PRINTLN\n");
    return;
  }

  if (o == end) {
    printf("END\n");
    return;
  }

  if (o == wrt) {
    printf("POP A\n");
    printf("PRINT A\n");
    return;
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
  }
  printf("PUSH C\n");
}
