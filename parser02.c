#include <stdio.h>
#include <stdlib.h>

extern int yyparse();
extern FILE *yyin;      /* 読み込むソースファイル */

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf ("argument error\n");
    exit(0);
  }

  yyin = fopen(argv[1], "r");
  if (yyin  == NULL) {
    printf ("%s file not found.\n", argv[1]);
    exit(0);
  }

  /* 構文解析スタート */
  if (yyparse() != 0) {
	 printf("parse error\n");
	 exit(0);
  } else {
	printf("parse success\n");
  }	
}
