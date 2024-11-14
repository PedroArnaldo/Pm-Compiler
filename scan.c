#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "scan.h"

char ctr = 1;

char peek_char(){
    return ctr;
}

void advance(){
    ctr = read_char();
}

void retreat(){
    unget_char(ctr);
}

void skip_space(){
    char c = peek_char();
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
        advance();
        c = peek_char();
    }
}

int isSpecialCharacter(char c) {
    char specialChars[] = {'=', '|', '>', '<', '&', '!'};
    int i;

    for (i = 0; i < 6; i++) {
        if (c == specialChars[i]) {
            return 1;
        }
    }
    return 0;
}

Token* read_number(){
    Token *t = (Token*) malloc(sizeof(Token));
    t->type = NUMBER;
    t->line = get_line();
    int i = 0;
    char c = peek_char();
    while (isdigit(c)){
        t->lexema[i++] = c;
        advance();
        c = peek_char();
    }
    if (c == '.'){
        t->lexema[i++] = c;
        advance();
        c = peek_char();
        if(!isdigit(c)){
            t->type = T_ERROR;
            return t;
        }
        t->type = REAL;
    } else {
        t->type = INT;
    }
    while (isdigit(c)){
        t->lexema[i++] = c;
        advance();
        c = peek_char();
    }
    t->lexema[i] = '\0';
    return t;
}

char *read_word(){
    char *word = (char*) malloc(sizeof(char) * 255);
    int i = 0;
    char c = peek_char();
    while (isalnum(c)){
        word[i++] = c;
        advance();
        c = peek_char();
    }
    word[i] = '\0';
    return word;
}

char* get_all_symbol(){
    char *word = (char*) malloc(sizeof(char) * 255);
    int i = 0;
    char c = peek_char();
    while (isSpecialCharacter(c)){
        word[i++] = c;
        advance();
        c = peek_char();
    }
    word[i] = '\0';
    return word;
}

int find_delimiters(char c){
    char delimiters[] = {'+', '-', '*', '/', '(', ')', '{', '}', ';', ','};
    for (int i = 0; i < 10; i++){
        if (c == delimiters[i]){
            return 1;
        }
    }
    return 0;
}

Token* get_next_token(){
    if (peek_char() == 1 && get_line() == 1){
        advance();
    }
    skip_space();
    char c = peek_char();

    if (c == EOF){
        Token *t = (Token*) malloc(sizeof(Token));
        t->type = T_EOF;
        t->line = get_line();
        strcpy(t->lexema, "EOF");
        return t;
    }


    if (isdigit(c)){
        return read_number();
    }   

    if (isalpha(c)){
        Token *t = (Token*) malloc(sizeof(Token));
        t->line = get_line();
        char *word = read_word();
        if (strcmp(word, "inteiro") == 0){
            t->type = INT;
        } else if (strcmp(word, "real") == 0){
            t->type = REAL;
        } else if (strcmp(word, "ler") == 0){
            t->type = READ;
        } else if (strcmp(word, "mostrar") == 0){
            t->type = SHOW;
        } else if (strcmp(word, "se") == 0){
            t->type = IF;
        } else if (strcmp(word, "entao") == 0){
            t->type = THEN;
        } else if (strcmp(word, "senao") == 0){
            t->type = ELSE;
        } else if (strcmp(word, "enquanto") == 0){
            t->type = WHILE;
        } else if (strcmp(word, "repita") == 0){
            t->type = REPEAT;
        } else if (strcmp(word, "ate") == 0){
            t->type = UNTIL;
        } else {
            t->type = IDENTIFIER;
        }
        strcpy(t->lexema, word);
        if (word != NULL){
            free(word);
        }
        return t;
    }


    if (find_delimiters(c)){
        Token *t = (Token*) malloc(sizeof(Token));
        t->line = get_line();
        if (c == '+'){
            t->type = PLUS;
            strcpy(t->lexema, "+");
        } else if (c == '-'){
            t->type = MINUS;
            strcpy(t->lexema, "-");
        } else if (c == '*'){
            t->type = MULT;
            strcpy(t->lexema, "*");
        } else if (c == '/'){
            t->type = DIV;
            strcpy(t->lexema, "/");
        } else if (c == '('){
            t->type = PAREN_L;
            strcpy(t->lexema, "(");
        } else if (c == ')'){
            t->type = PAREN_R;
            strcpy(t->lexema, ")");
        } else if (c == '{'){
            t->type = KEY_OPEN;
            strcpy(t->lexema, "{");
        } else if (c == '}'){
            t->type = KEY_CLOSE;
            strcpy(t->lexema, "}");
        } else if (c == ';'){
            t->type = SEPARATOR_CMD;
            strcpy(t->lexema, ";");
        } else if (c == ','){
            t->type = SEPARATOR_ID;
            strcpy(t->lexema, ",");
        }
        advance();
        return t;
    }

    char *word = get_all_symbol();
    Token *t = (Token*) malloc(sizeof(Token));
    t->line = get_line();
    if (strcmp(word, "&&") == 0){
        t->type = AND;
        strcpy(t->lexema, word);
    } else if (strcmp(word, "||") == 0){
        t->type = OR;
        strcpy(t->lexema, word);
    } else if (strcmp(word, "<") == 0){
        t->type = LESS;
        strcpy(t->lexema, word);
    } else if (strcmp(word, ">") == 0){
        t->type = GREATER;
        strcpy(t->lexema, word);
    } else if (strcmp(word, "<=") == 0){
        t->type = LESS_EQUAL;
        strcpy(t->lexema, word);
    } else if (strcmp(word, ">=") == 0){
        t->type = GREATER_EQUAL;
        strcpy(t->lexema, word);
    } else if (strcmp(word, "==") == 0){
        t->type = EQUAL;
        strcpy(t->lexema, word);
    } else if (strcmp(word, "!=") == 0){
        t->type = NOT_EQUAL;
        strcpy(t->lexema, word);
    } else if (strcmp(word, "=") == 0){
        t->type = ASSIGN;
        strcpy(t->lexema, word);
    } else {
        t->type = T_ERROR;
        strcpy(t->lexema, "ERROR");
    }

    if(word != NULL){
        free(word);
    }

    return t;
}