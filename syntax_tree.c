#include "utils.h"
#include "syntax_tree.h"


SyntaxTreeNode *create_node(int type, const char *lexeme) {
    SyntaxTreeNode *node = (SyntaxTreeNode *)malloc(sizeof(SyntaxTreeNode));
    node->type = type;
    node->lexeme = lexeme ? strdup(lexeme) : NULL;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

void add_child(SyntaxTreeNode *parent, SyntaxTreeNode *child) {
    parent->child_count++;
    parent->children = (SyntaxTreeNode **)realloc(
        parent->children, parent->child_count * sizeof(SyntaxTreeNode *));
    parent->children[parent->child_count - 1] = child;
}

void free_tree(SyntaxTreeNode *node) {
    if (node) {
        for (int i = 0; i < node->child_count; i++) {
            free_tree(node->children[i]);
        }
        free(node->children);
        free(node->lexeme);
        free(node);
    }
}

void print_tree(SyntaxTreeNode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");
    printf("Node(type: %d, lexeme: %s)\n", node->type, node->lexeme ? node->lexeme : "NULL");

    for (int i = 0; i < node->child_count; i++) {
        print_tree(node->children[i], depth + 1);
    }
}
