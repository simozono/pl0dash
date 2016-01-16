#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern void pl0_error(char *type, char *lexeme,
		      int line, char *error_message);
extern void info_ref_symbol(int ptr, int cur_line_no);

