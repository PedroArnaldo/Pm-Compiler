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

int check_symbol(const char *name) {
    for (int i = symbol_count - 1; i >= 0; i--) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].type;
        }
    }
    return -1;
}

void add_symbol(const char *name, int type) {
    Symbol *symbol = (Symbol *) malloc(sizeof(Symbol));
    strcpy(symbol->name, name);
    symbol->type = type;
    symbol_table = (Symbol *) realloc(symbol_table, (symbol_count + 1) * sizeof(Symbol));
    symbol_table[symbol_count++] = *symbol;
}


void print_table_symbol() {
    printf("Tabela de Símbolos:\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("Name: %s, Type: %d\n", symbol_table[i].name, symbol_table[i].type);
    }
}

SyntaxTreeNode *parse() {
    token = get_next_token();
    SyntaxTreeNode *root = programa();
    printf("Árvore Sintática Gerada:\n");
    print_tree(root, 0);
    print_table_symbol();
    return root;
}

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
            add_symbol(get_current_token()->lexema, get_current_token()->type);
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
        case UNTIL:
            printf("REPEAT Estou aqui type %d lexema %s\n", get_current_token()->type, get_current_token()->lexema);
            add_child(node, comando_repeat());
            break;
        default:
            printf("Default Estou aqui type %d lexema %s line -> %d\n", get_current_token()->type, get_current_token()->lexema, get_current_token()->t_line);
            syntax_error("Comando desconhecido");
    }
    return node;
}

SyntaxTreeNode *comando_leitura() {
    SyntaxTreeNode *node = create_node(3, "ler");
    advance_token();
    if (get_current_token()->type == PAREN_L) {
        advance_token(); 
        if (get_current_token()->type == IDENTIFIER) {
            add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
            if (check_symbol(get_current_token()->lexema) == -1) {
                syntax_error("Variável não declarada");
            }
            advance_token(); 
            if (get_current_token()->type == PAREN_R) {
                advance_token();
                if (get_current_token()->type == SEPARATOR_CMD) {
                    advance_token(); 
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
    advance_token();
    if (get_current_token()->type == PAREN_L) {
        advance_token(); 
        if (get_current_token()->type == IDENTIFIER) {
            add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
            if (check_symbol(get_current_token()->lexema) == -1) {
                syntax_error("Variável não declarada");
            }
            advance_token(); 
            if (get_current_token()->type == PAREN_R) {
                advance_token(); 
                if (get_current_token()->type == SEPARATOR_CMD) {
                    advance_token(); 
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
    SyntaxTreeNode *node = create_node(5, "atribuicao");
    if (get_current_token()->type == IDENTIFIER) {
        if (check_symbol(get_current_token()->lexema) == -1) {
            syntax_error("Variável não declarada");
        }
        add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
        advance_token(); 
        if (get_current_token()->type == ASSIGN) {
            advance_token(); 
            add_child(node, expressao());
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
    advance_token(); 
    add_child(node, expressao());
    if (get_current_token()->type == THEN) {
        advance_token(); 
        add_child(node, comando());
        if (get_current_token()->type == ELSE) {
            advance_token(); 
            add_child(node, comando());
        }
    } else {
        syntax_error("'então' esperado após expressão de 'se'");
    }
    return node;
}

SyntaxTreeNode *expressao() {
    SyntaxTreeNode *node = create_node(7, "expressao");
    add_child(node, termo());
    while (get_current_token()->type == PLUS || get_current_token()->type == MINUS || get_current_token()->type == OR || get_current_token()->type == AND || get_current_token()->type == NOT_EQUAL || get_current_token()->type == EQUAL || get_current_token()->type == LESS || get_current_token()->type == GREATER || get_current_token()->type == LESS_EQUAL || get_current_token()->type == GREATER_EQUAL) {
        add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
        advance_token(); 
        add_child(node, termo());
    }
    return node;
}

SyntaxTreeNode *termo() {
    SyntaxTreeNode *node = create_node(8, "termo");
    add_child(node, fator());
    while (get_current_token()->type == MULT || get_current_token()->type == DIV) {
        add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
        advance_token();
        add_child(node, fator());
    }
    return node;
}

SyntaxTreeNode *fator() {
    SyntaxTreeNode *node = create_node(9, "fator");

    if (get_current_token()->type == IDENTIFIER || get_current_token()->type == INT || get_current_token()->type == REAL) {
        add_child(node, create_node(get_current_token()->type, get_current_token()->lexema));
        advance_token(); 
    } else if (get_current_token()->type == PAREN_R) {
        advance_token(); 
        add_child(node, expressao());
        if (get_current_token()->type == PAREN_L) {
            advance_token(); 
        } else {
            syntax_error("')' esperado após expressão");
        }
    }else {
        syntax_error("Identificador, número ou '(' esperado no fator");
    }

    if (get_current_token()->type == SEPARATOR_CMD) {
       advance_token(); 
    }

    return node;
}

SyntaxTreeNode *comando_while() {
    SyntaxTreeNode *node = create_node(10, "enquanto");
    advance_token(); 
    add_child(node, expressao());
    add_child(node, comando());
    return node;
}

SyntaxTreeNode *comando_repeat() {
    SyntaxTreeNode *node = create_node(11, "repita");
    advance_token(); 
    if (get_current_token()->type == KEY_OPEN || get_current_token()->type == KEY_CLOSE){
        advance_token();
    }
    while (get_current_token()->type != UNTIL){
        if (get_current_token()->type == T_EOF)
            syntax_error("'até' esperado após o comando de repetição");
        if (get_current_token()->type == KEY_OPEN || get_current_token()->type == KEY_CLOSE){
            advance_token();
            break;
        }
        add_child(node, comando());
    }
     if (get_current_token()->type == KEY_OPEN || get_current_token()->type == KEY_CLOSE)
        advance_token(); 
    if (get_current_token()->type == UNTIL) {
        advance_token(); 
        add_child(node, expressao());
    } else if(get_current_token()->type == SEPARATOR_CMD){
        advance_token();
    } 
    else {
        syntax_error("'até' esperado após comando de repetição");
    }
    return node;
}
