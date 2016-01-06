#include <stdio.h>
#include <stdlib.h>

extern int yyparse();
extern FILE *yyin;      /* 読み込むソースファイル */

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf ("argument error\n");
    exit(EXIT_FAILURE);
  }

  yyin = fopen(argv[1], "r");
  if (yyin  == NULL) {
    printf ("%s file not found.\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  /* 構文解析スタート */
  if (yyparse() != 0) {
	 printf("parse error\n");
	 exit(EXIT_FAILURE);
  } else {
	printf("parse success\n");
  }	
}
