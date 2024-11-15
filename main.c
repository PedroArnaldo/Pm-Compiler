#include "utils.h"


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    set_file(file);
    
    Token* t = get_next_token();
    while (t->type != T_EOF)
    {
        printf("Token: %d -  Lexema: %s - Line: %d\n", t->type, t->lexema, t->t_line);
        t = get_next_token();
        if (t->type == T_ERROR)
           error_lexico(t->t_line);
    }

    //passos 
        // inicar a leitura do arquivo
        //fazer a analise lexica
        //fazer a analise sintaica
        //fazer a analise semantica
}