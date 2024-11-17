#include "utils.h"
#include "parse.h"

Token *token;
Symbol *symbol_table;
int symbol_count = 0;

Token* get_current_token() {
    return token;
}

void advance_token() {
    token = get_next_token();
}

void add_symbol(const char *name, int type, int scope) {
    Symbol *symbol = (Symbol *) malloc(sizeof(Symbol));
    strcpy(symbol->name, name);
    symbol->type = type;
    symbol->scope = scope;
    symbol_table = (Symbol *) realloc(symbol_table, (symbol_count + 1) * sizeof(Symbol));
    symbol_table[symbol_count++] = *symbol;
}

int find_symbol(const char *name, int scope) {
    for (int i = symbol_count - 1; i >= 0; i--) {
        if (strcmp(symbol_table[i].name, name) == 0 && symbol_table[i].scope == scope) {
            return symbol_table[i].type;
        }
    }
    return -1;
}

// Função principal
SyntaxTreeNode *parse() {
    token = get_next_token();
    SyntaxTreeNode *root = programa();
    printf("Árvore Sintática Gerada:\n");
    print_tree(root, 0);
    return root;
}

// Funções da gramática
SyntaxTreeNode *programa() {
    SyntaxTreeNode *node = create_node(0, "programa");
    while (get_current_token()->type != T_EOF || get_current_token()->type == IDENTIFIER) {
        if (get_current_token()->type == REAL || get_current_token()->type == INT) {
            add_child(node, declaracao());
        } else {
            add_child(node, comando());
        }
    }
    return node;
}

SyntaxTreeNode *declaracao() {
    SyntaxTreeNode *node = create_node(1, "declaracao");
    if (get_current_token()->type == INT || get_current_token()->type == REAL) {
        add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
        advance_token();
        if (get_current_token()->type == IDENTIFIER) {
            add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
            advance_token();
            if (get_current_token()->type == SEPARATOR_CMD) {
                advance_token();
            } else {
                syntax_error("';' esperado no final da declaração");
            }
        } else {
            syntax_error("Identificador esperado após tipo");
        }
    } else {
        syntax_error("Tipo esperado no início da declaração");
    }
    return node;
}

SyntaxTreeNode *comando() {
    SyntaxTreeNode *node = create_node(2, "comando");
    switch (get_current_token()->type) {
        case READ:
            add_child(node, comando_leitura());
            break;
        case SHOW:
            add_child(node, comando_escrita());
            break;
        case IDENTIFIER:
            add_child(node, comando_atribuicao());
            break;
        case IF:
            add_child(node, comando_se());
            break;
        case WHILE:
            add_child(node, comando_while());
            break;
        case REPEAT:
            add_child(node, comando_repeat());
            break;
        default:
            syntax_error("Comando desconhecido");
    }
    return node;
}

SyntaxTreeNode *comando_leitura() {
    SyntaxTreeNode *node = create_node(3, "ler");
    advance_token(); // Consome 'ler'
    if (get_current_token()->type == PAREN_L) {
        advance_token(); // Consome '('
        if (get_current_token()->type == IDENTIFIER) {
            add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
            advance_token(); // Consome identificador
            if (get_current_token()->type == PAREN_R) {
                advance_token(); // Consome ')'
                if (get_current_token()->type == SEPARATOR_CMD) {
                    advance_token(); // Consome ';'
                } else {
                    syntax_error("';' esperado após ')'");
                }
            } else {
                syntax_error("')' esperado após identificador");
            }
        } else {
            syntax_error("Identificador esperado dentro de 'ler()'");
        }
    } else {
        syntax_error("'(' esperado após 'ler'");
    }
    return node;
}

SyntaxTreeNode *comando_escrita() {
    SyntaxTreeNode *node = create_node(4, "mostrar");
    advance_token(); // Consome 'mostrar'
    if (get_current_token()->type == PAREN_L) {
        advance_token(); // Consome '('
        if (get_current_token()->type == IDENTIFIER) {
            add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
            advance_token(); // Consome identificador
            if (get_current_token()->type == PAREN_R) {
                advance_token(); // Consome ')'
                if (get_current_token()->type == SEPARATOR_CMD) {
                    advance_token(); // Consome ';'
                } else {
                    syntax_error("';' esperado após ')'");
                }
            } else {
                syntax_error("')' esperado após identificador");
            }
        } else {
            syntax_error("Identificador esperado dentro de 'mostrar()'");
        }
    } else {
        syntax_error("'(' esperado após 'mostrar'");
    }
    return node;
}

SyntaxTreeNode *comando_atribuicao() {
    printf("ATRIBUICAAO Estou aqui type %d lexema %s\n", get_current_token()->type, get_current_token()->lexema);
    SyntaxTreeNode *node = create_node(5, "atribuicao");
    if (get_current_token()->type == IDENTIFIER) {
        add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
        advance_token(); // Consome identificador
        if (get_current_token()->type == ASSIGN) {
            advance_token(); // Consome '='
            add_child(node, expressao());
            if (get_current_token()->type == SEPARATOR_CMD) {
                advance_token(); // Consome ';'
            } else {
                syntax_error("';' esperado após expressão de atribuição");
            }
        } else {
            syntax_error("'=' esperado após identificador");
        }
    } else {
        syntax_error("Identificador esperado no início da atribuição");
    }
    return node;
}

SyntaxTreeNode *comando_se() {
    SyntaxTreeNode *node = create_node(6, "se");
    advance_token(); // Consome 'se'
    add_child(node, expressao());
    printf("Estou aqui ENTAO type %d lexema %s\n", get_current_token()->type, get_current_token()->lexema);
    if (get_current_token()->type == THEN) {
        advance_token(); // Consome 'então'
        add_child(node, comando());
        if (get_current_token()->type == ELSE) {
            advance_token(); // Consome 'senão'
            add_child(node, comando());
        }
    } else {
        syntax_error("'então' esperado após expressão de 'se'");
    }
    return node;
}

SyntaxTreeNode *expressao() {
    SyntaxTreeNode *node = create_node(7, "expressao");
    printf("Estou aqui type %d lexema %s\n", get_current_token()->type, get_current_token()->lexema);
    add_child(node, termo());
    while (get_current_token()->type == PLUS || get_current_token()->type == MINUS) {
        add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
        advance_token(); // Consome operador
        add_child(node, termo());
    }
    return node;
}

SyntaxTreeNode *termo() {
    SyntaxTreeNode *node = create_node(8, "termo");
    add_child(node, fator());
    while (get_current_token()->type == MULT || get_current_token()->type == DIV) {
        add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
        advance_token(); // Consome operador
        add_child(node, fator());
    }
    return node;
}

SyntaxTreeNode *fator() {
    SyntaxTreeNode *node = create_node(9, "fator");
    if (get_current_token()->type == IDENTIFIER || get_current_token()->type == INT || get_current_token()->type == REAL) {
        add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
        advance_token(); // Consome identificador ou número
    } else if (get_current_token()->type == PAREN_R) {
        advance_token(); // Consome '('
        add_child(node, expressao());
        if (get_current_token()->type == PAREN_L) {
            advance_token(); // Consome ')'
        } else {
            syntax_error("')' esperado após expressão");
        }
    } else {
        syntax_error("Identificador, número ou '(' esperado no fator");
    }
    return node;
}

SyntaxTreeNode *comando_while() {
    SyntaxTreeNode *node = create_node(10, "enquanto");
    advance_token(); // Consome 'enquanto'
    add_child(node, expressao());
    add_child(node, comando());
    return node;
}

SyntaxTreeNode *comando_repeat() {
    SyntaxTreeNode *node = create_node(11, "repita");
    advance_token(); // Consome 'repita'
    add_child(node, comando());
    if (get_current_token()->type == UNTIL) {
        advance_token(); // Consome 'até'
        add_child(node, expressao());
    } else {
        syntax_error("'até' esperado após comando de repetição");
    }
    return node;
}
