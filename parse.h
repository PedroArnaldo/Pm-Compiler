#ifndef PARSE_H
#define PARSE_H

#include "syntax_tree_node.h"
#include "utils.h"

typedef struct {
    char name[255];
    int type;
    int scope;
} Symbol;

// Função principal para iniciar o parsing
SyntaxTreeNode *parse();

// Funções auxiliares para cada não-terminal
SyntaxTreeNode *programa();
SyntaxTreeNode *declaracao();
SyntaxTreeNode *comando();
SyntaxTreeNode *comando_leitura();
SyntaxTreeNode *comando_escrita();
SyntaxTreeNode *comando_atribuicao();
SyntaxTreeNode *comando_se();
SyntaxTreeNode *expressao();
SyntaxTreeNode *termo();
SyntaxTreeNode *fator();
SyntaxTreeNode *comando_repeat();
SyntaxTreeNode *comando_while();

#endif