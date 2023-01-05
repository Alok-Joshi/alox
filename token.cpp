#include<iostream>
#include "token.h"


token:: token(string &lexeme, token_type &type, int &line_number,int &literalvalue){

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

    std::cout<<"lexeme = "<<lexeme<<" type = "<<type<<" linenumber = "<<line_number<<endl;
}





