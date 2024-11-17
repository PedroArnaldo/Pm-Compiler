#include "utils.h"

FILE *file;
char caracter;
int line = 1;

void unget_char(char c)
{
	ungetc(c, file);
}

void set_file(FILE *f)
{
	file = f;
}

char read_char()
{
	char c;

	c = fgetc(file);
	if (c == '\n')
		line++;

	return c;
}

int get_line()
{
	return line;
}

void error_lexico(int line_in){
	printf("Error lexico line: %d \n", line_in);
	exit(EXIT_FAILURE);
}

void syntax_error(char *msg)
{
	printf("Erro de sintaxe na linha %d: %s\n", line, msg);
	exit(EXIT_FAILURE);
}