#ifndef SYNTAX_TREE_NODE_H
#define SYNTAX_TREE_NODE_H

typedef struct SyntaxTreeNode{
    int type; // Tipo do nó (Token ou regra da gramática)
    char *lexeme; // Lexema associado (para identificadores, números, etc.)
    struct SyntaxTreeNode **children; // Filhos do nó
    int child_count; // Número de filhos
} SyntaxTreeNode;


#endif // SYNTAX_TREE_NODE_H