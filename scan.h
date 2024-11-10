#ifndef SCAN_H
#define SCAN_H

#include <stdio.h>

typedef enum {   
    IDENTIFIER,     //variavel 0
    NUMBER,         // 1
    PLUS,           // + 2
    MINUS,          // - 3
    MULT,           // * 4  
    DIV,            // / 5
    AND,            // && 6
    OR,             // || 7
    LESS,           // <  8  
    GREATER,        // >  9
    LESS_EQUAL,     // <= 10
    GREATER_EQUAL,  // >= 11
    EQUAL,          // == 12
    NOT_EQUAL,      // != 13
    ASSIGN,         // =  14
    SYMBOL,         //{ } 15
    STR,            //"word" 16
    PAREN_R,        // (  17
    PAREN_L,        // )  18
    IF,             // if  19 
    THEN,           // entao  20
    ELSE,           // else  21
    REPEAT,         // repita  22
    UNTIL,          // ate      23 
    WHILE,          // enquanto 24
    INT,            // inteiro  25
    REAL,           // real     26
    READ,           // ler somenente numeros 27
    SHOW,           // mostrar  28
    SEPARATOR_CMD,      // ;  29
    SEPARATOR_ID,      // ,   30
    T_EOF,        // 31 
    T_ERROR         // 32
} TokenType;

typedef struct{
    TokenType type;
    char lexema[255];
    int line;
} Token;

Token* get_next_token();
void print_char();

#endif // SCAN_H