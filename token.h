//token.h

#ifndef TOKEN_H
#define TOKEN_H
#include<string>
#include<iostream>

namespace tok {
typedef enum {
    // Single-character tokens.
  LEFT_PAREN, //0
  RIGHT_PAREN, //1
  LEFT_BRACE, //2
  RIGHT_BRACE, //3
  COMMA, //4
  DOT, //5 
  MINUS, //6
  PLUS, //7
  SEMICOLON, //8
  COLON,//9
  SLASH, //10
  STAR, //11

  // Types
  STRING_TYPE,//12
  NUMBER_TYPE,//13
  // One or two character tokens.
  BANG, //14
  BANG_EQUAL, //15
  EQUAL, //16
  EQUAL_EQUAL, //17
  GREATER, //18
  GREATER_EQUAL, //19
  LESS, //20
  LESS_EQUAL, //21

  // Literals.
  IDENTIFIER, //22
  STRING, //23
  NUMBER, //24

  // Keywords.
  AND, //25
  CLASS, //26
  ELSE, //27
  FALSE, //28
  FUN, //29
  FOR, //30
  IF, //31
  NIL, //32
  OR, //33
  PRINT, //34
  RETURN, //35
  SUPER, //36
  THIS, //37
  TRUE, //38
  VAR, //39
  WHILE, //40
  END_OF_FILE, //41

  //additional types
  VOID_TYPE,
  FUNCTION_TYPE

} token_type;



class token{
    public:
    std::string lexeme;
    token_type type;
    int line_number;
    std::string string_literal_value;
    double int_literal_value;
   
    token();
    token(std::string &lexeme, token_type &type,int &line_number,std::string &literalvalue);
    token(std::string &lexeme, token_type &type,int &line_number,double &literalvalue);
    token(std::string &lexeme, token_type &type, int &line_number);
    bool operator <(const token & tok2);
    void print_token();

};

}

#endif

