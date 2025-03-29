#include<iostream>
#include "token.h"

using namespace tok;
using namespace std;

token:: token(){};
token:: token(string &lexeme, token_type &type, int &line_number,double &literalvalue){

        this->lexeme = lexeme;
        this->type = type;
        this->line_number = line_number;
        this->int_literal_value = literalvalue;
}

token:: token(string &lexeme, token_type &type, int &line_number,string &literalvalue){

        this->lexeme = lexeme;
        this->type = type;
        this->line_number = line_number;
        this->string_literal_value = literalvalue;
}
token:: token(string &lexeme, token_type &type, int &line_number){

        this->lexeme = lexeme;
        this->type = type;
        this->line_number = line_number;
}


void token:: print_token(){

    std::cout<<lexeme;
}





