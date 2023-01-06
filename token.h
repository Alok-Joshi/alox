//token.h

#ifndef TOKEN_H
#define TOKEN_H
#include<string>
#include<iostream>

namespace tok {
typedef enum {
    // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,END_OF_FILE

} token_type;



class token{

    std::string lexeme;
    token_type type;
    int line_number;
    std::string string_literal_value;
    int int_literal_value;
    
    public:
    token(std::string &lexeme, token_type &type,int &line_number,std::string &literalvalue);
    token(std::string &lexeme, token_type &type,int &line_number,int &literalvalue);
    token(std::string &lexeme, token_type &type, int &line_number);
    void print_token();

};

}

#endif

