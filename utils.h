#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

char **split(const char *text, char delimiter);
char read_char();
int get_line();
void set_file(FILE *f);

#endif // UTILS_H