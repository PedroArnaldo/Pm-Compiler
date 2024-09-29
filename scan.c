#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

typedef enum {
    KEY_WORD,
    IDENTIFIER,
    NUMBER,
    OPERADOR,
    SYMBOL,
    T_EOF,
} TokenType;

typedef struct{
    TokenType type;
    char lexema[255];
    int line;
} Token;

typedef struct ListToken_T{
    Token *tokens;
    struct ListToken_T *next;
} ListToken;

void push(ListToken **head, Token *token){
    
    ListToken *current;

    current = malloc(sizeof(ListToken));

    current->tokens = token;
    current->next = *head;
    *head = current;
}

const char *keywords[] = {"inteiro", "ler", "se", "entao", "senao", "mostrar", "real", "enquanto", "repita", "ate"};

TokenType check_keyword(const char *str){
    for (int i = 0; i < 10; i++){
        if(strcmp(str, keywords[i]) == 0)
            return KEY_WORD;
    }
    return IDENTIFIER;
}

TokenType is_operator(char c){
    return (c == '+' || c == '-' || c == '*' || c =='/' || c == '&' || c == '|' || c == '<' || c == '!' || c == '>' );
}

int is_symbol(char c) {
    return (c == ';' || c == '(' || c == ')' || c == ',' || c == '{' || c == '}');
}

ListToken **head;

void get_next_token(char *word, int line){

    int i = 0;
    int t = 0;

    while(word[i] != '\0'){
        int i_lexema = 0;
        Token *token = malloc(sizeof(Token));
        memset(token, 0, sizeof(token));
        while (isspace(word[i]) || word[i] == '\t' || word[i] == '\n')
            i++;
        
        if (word[i] == EOF){
            token->type = T_EOF;
            strcpy(token->lexema, "EOF");
            printf("Token: [ %s ] - Tipo: %d - line %d\n", token->lexema, token->type, line);
            continue;
        }

        if (isalpha(word[i])){
            while(isalnum(word[i])){
                token->lexema[i_lexema++] = word[i++];
            }
            token->lexema[i_lexema] = '\0';
            token->type = check_keyword(token->lexema); 
            printf("Token: [ %s ] - Tipo: %d - line %d\n", token->lexema, token->type, line);
            continue;
        }
        
        
        if(isdigit(word[i])){
            while(isdigit(word[i])){
                token->lexema[i_lexema++] = word[i++];
            }
            token->lexema[i_lexema] = '\0';
            token->type = NUMBER;
            printf("Token: [ %s ] - Tipo: %d - line %d\n", token->lexema, token->type, line);
            continue;
        }
        if(is_operator(word[i])){
            token->lexema[i_lexema++] = word[i++];
            token->type = OPERADOR;
            printf("Token: [ %s ] - Tipo: %d - line %d\n", token->lexema, token->type, line);
            continue;
        }

        if(is_symbol(word[i])){
            token->lexema[i_lexema++] = word[i++];
            token->lexema[i_lexema] = '\0';
            token->type = SYMBOL;
            printf("Token: [ %s ] - Tipo: %d - line %d\n", token->lexema, token->type, line);
            continue;
        }
    }
    
}

int main() {
    char **words;

    Token token;
    int i = 0; 
    
    printf("INICIO %p\n", &head);
    char *text = "inteiro i;\nler(i);\nse i>10\nentao\nmostrar(i)\nsenao\nmostrar(10);\nmostrar (-1);\n";
    char **line = split(text, '\n');

    while(line[i]){
        words = split(line[i], ' ');  
        int j = 0;
        while (words[j]){
            get_next_token(words[j], i);
            j++;
        }
        i++;
    }
    return 0;
}