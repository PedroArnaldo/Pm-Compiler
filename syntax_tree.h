#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include "syntax_tree_node.h"

// Função para criar um novo nó da árvore
SyntaxTreeNode *create_node(int type, const char *lexeme);

// Função para adicionar um filho a um nó
void add_child(SyntaxTreeNode *parent, SyntaxTreeNode *child);

// Função para liberar memória da árvore
void free_tree(SyntaxTreeNode *node);

// Função para imprimir a árvore (para depuração)
void print_tree(SyntaxTreeNode *node, int depth);

#endif
