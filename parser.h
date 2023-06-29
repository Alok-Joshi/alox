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
    bool inside_function = false;
    int current;

    public:
    parser(std::vector<tok::token> &tokens);
    ast::expression* parse_expression();
    ast::expression* parse_assignment();
    ast::expression* parse_logical_or();
    ast::expression* parse_logical_and();
    ast::expression* parse_equality();
    ast::expression* parse_comparison();
    ast::expression* parse_addsub();
    ast::expression* parse_multdiv();
    ast::expression* parse_unary();
    ast::expression* parse_literal();
    ast::expression* parse_call();
    std::vector<ast::statement*> parse_program();
    ast::statement* parse_declaration();
    ast::statement* parse_expression_statement();
    ast::statement* parse_print_statement();
    ast::statement* parse_number_input_statement();
    ast::statement* parse_string_input_statement();
    ast::statement* parse_conditional_statement();
    ast::statement* parse_while_statement();
    ast::statement* parse_for_statement();
    ast::statement* parse_block_statement();
    std::pair<tok::token,tok::token_type>  parse_function_parameter();
    ast::statement* parse_function_declaration_statement();
    ast::statement* parse_return_statement();
    ast::statement* parse_statement();
    ast::statement* parse_class_declaration_statement();
    ast::statement* parse_class_method();
    tok:: token get_operator();
    void synchronise();
    tok:: token get_literal();
    tok:: token consume_token(std:: unordered_set<tok::token_type> &valid_types);
    tok:: token consume_token(tok::token_type tt);
    bool match(std:: unordered_set<tok::token_type> &valid_types);
    bool match(tok:: token_type tt);
    tok::token peak();



};



#endif





