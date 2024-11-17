#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "syntax_tree_node.h"
#include "scan.h"
#include "parse.h"
#include "syntax_tree.h"

char read_char();
int get_line();
void set_file(FILE *f);
void error_lexico(int line);
void unget_char(char c);
void syntax_error(char *msg);

#endif // UTILS_H