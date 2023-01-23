#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include<unordered_set>
#include "token.h"
#include "ast.h"


class parser{
    
    /*Every rule of the grammar is a seperate function. Recursive descent */
    private:
    std::vector<tok::token> tokens;
    int current;

    public:
    parser(std::vector<tok::token> &tokens);

    ast::expression* parse_expression();
    ast::expression* parse_equality();
    ast::expression* parse_comparison();
    ast::expression* parse_addsub();
    ast::expression* parse_multdiv();
    ast::expression* parse_unary();
    ast::expression* parse_literal();
    tok:: token get_operator();
    tok:: token get_literal();
    bool match(std:: unordered_set<tok::token_type> &valid_types);


};



#endif





