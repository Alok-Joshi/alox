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
    bool error_status = false;
    parser(std::vector<tok::token> &tokens);
    std::unique_ptr<ast::expression> parse_expression();
    std::unique_ptr<ast::expression> parse_assignment();
    std::unique_ptr<ast::expression> parse_logical_or();
    std::unique_ptr<ast::expression> parse_logical_and();
    std::unique_ptr<ast::expression> parse_equality();
    std::unique_ptr<ast::expression> parse_comparison();
    std::unique_ptr<ast::expression> parse_addsub();
    std::unique_ptr<ast::expression> parse_multdiv();
    std::unique_ptr<ast::expression> parse_unary();
    std::unique_ptr<ast::expression> parse_literal();
    std::unique_ptr<ast::expression> parse_call();
    std::vector<std::unique_ptr<ast::statement>> parse_program();
    std::unique_ptr<ast::statement> parse_declaration();
    std::unique_ptr<ast::statement> parse_expression_statement();
    std::unique_ptr<ast::statement> parse_print_statement();
    std::unique_ptr<ast::statement> parse_number_input_statement();
    std::unique_ptr<ast::statement> parse_string_input_statement();
    std::unique_ptr<ast::statement> parse_conditional_statement();
    std::unique_ptr<ast::statement> parse_while_statement();
    std::unique_ptr<ast::statement> parse_for_statement();
    std::unique_ptr<ast::statement> parse_block_statement();
    std::pair<tok::token,tok::token_type>  parse_function_parameter();
    std::unique_ptr<ast::statement> parse_function_declaration_statement();
    std::unique_ptr<ast::statement> parse_return_statement();
    std::unique_ptr<ast::statement> parse_statement();
    std::unique_ptr<ast::statement> parse_class_declaration_statement();
    std::unique_ptr<ast::statement> parse_class_method();
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





