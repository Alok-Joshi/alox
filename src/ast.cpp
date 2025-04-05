#include <functional>
#include <iostream>
#include<iomanip>
#include "ast.h"
#include "token.h"
#include "environment.h"


using namespace ast;
using namespace std;
using namespace tok;


//CONSTRUCTORS
print_statement:: print_statement(std::unique_ptr<expression> exp,int line_number): exp(std::move(exp)),statement(line_number) {};
expression_statement:: expression_statement(std::unique_ptr<expression> exp,int line_number): statement(line_number), exp(std::move(exp)) {};
declaration_statement:: declaration_statement(std::unique_ptr<expression> exp,token variable_name,token_type variable_type,int line_number): statement(line_number), exp(std::move(exp)),variable_name(variable_name), variable_type(variable_type) {};
conditional_statement:: conditional_statement(std::unique_ptr<expression> expr, unique_ptr<statement> if_statements, unique_ptr<statement> else_statements,int line_number): statement(line_number),expr(std::move(expr)), if_statements(std::move(if_statements)),else_statements(std::move(else_statements)) {};
block_statement:: block_statement(vector<std::unique_ptr<statement>> statements,int line_number): statement(line_number),statements(std::move(statements)) {};
while_statement:: while_statement(std::unique_ptr<expression> expr, std::unique_ptr<statement> statements,int line_number): statement(line_number),expr(std::move(expr)), statements(std::move(statements)) {};
for_statement:: for_statement(std::unique_ptr<statement> part1, std::unique_ptr<expression> part2, std::unique_ptr<expression> part3, std::unique_ptr<statement> statements,int line_number): statement(line_number),part1(std::move(part1)), part2(std::move(part2)), part3(std::move(part3)), statements(std::move(statements)) {};
function_call_expression:: function_call_expression(token function_name,vector<std::unique_ptr<expression>> arguments,int line_number): expression(line_number),function_name(function_name), arguments(std::move(arguments)) {};
function_declaration_statement::function_declaration_statement(tok::token function_name, vector<pair<tok::token,tok::token_type>> &parameters, std::unique_ptr<statement> block,token_type return_type,int line_number): statement(line_number),function_name(function_name), parameters(parameters), block(std::move(block)), return_type(return_type) {};
return_statement:: return_statement(std::unique_ptr<expression> return_exp,int line_number): statement(line_number),return_exp(std::move(return_exp)) {};
class_declaration_statement:: class_declaration_statement(tok:: token variable_name, std::vector<std::unique_ptr<statement>> methods,int line_number): statement(line_number),variable_name(variable_name), methods(std::move(methods)) {}
input_statement:: input_statement(token input_reciever_variable,token_type input_type,int line_number): statement(line_number),input_reciever_variable(input_reciever_variable), input_type(input_type) {};
expression:: expression(int line_number): line_number(line_number) {};
statement:: statement(int line_number): line_number(line_number) {};
binary_expression:: binary_expression(std::unique_ptr<expression> left, token& optr, std::unique_ptr<expression> right,int line_number): left(std::move(left)), optr(optr), right(std::move(right)), expression(line_number) {}
unary_expression:: unary_expression(token &optr,std::unique_ptr<expression> right,int line_number): optr(optr),right(std::move(right)),expression(line_number) {}
logical_expression:: logical_expression(std::unique_ptr<expression> left, token&optr, std::unique_ptr<expression> right,int line_number): binary_expression(std::move(left),optr,std::move(right),line_number) {}
literal_expression:: literal_expression(token &literal,int line_number): literal(literal), expression(line_number) {}
variable_literal_expression:: variable_literal_expression(token  &variable_name,int line_number): variable_name(variable_name), expression(line_number) {}

void binary_expression:: print_expression() {

    cout<<"expr( ";
    this->left->print_expression();
    cout<<", ";
    this->optr.print_token();
    cout<<", ";
    this->right->print_expression();
    cout<<" )";

}

void unary_expression:: print_expression() {

    cout<<"expr( ";
    this->optr.print_token();
    cout<<", ";
    this->right->print_expression();
    cout<<" )";

}

token variable_literal_expression:: get_variable_name() {

    return this->variable_name;

}

void variable_literal_expression:: print_expression() {

    cout<<"expr( ";
    this->variable_name.print_token();
    cout<<" )";
}

void literal_expression:: print_expression() {

    cout<<"expr( ";
    this->literal.print_token();
    cout<<") ";

}

void function_call_expression:: print_expression() {};
void logical_expression:: print_expression() {

    cout<<"expr( ";
    this->left->print_expression();
    cout<<", ";
    this->optr.print_token();
    cout<<", ";
    this->right->print_expression();
    cout<<" )";

}

//ACCEPT implementation
any conditional_statement:: accept(ast::visitor* vt) {
    vt->visit_conditional_statement(this);
    return 0;
}
any block_statement :: accept(ast::visitor* vt) {
    vt->visit_block_statement(this);
    return 0;
}
any while_statement :: accept(ast::visitor* vt) {
    vt->visit_while_statement(this);
    return 0;
}
any for_statement :: accept(ast::visitor* vt) {
    vt->visit_for_statement(this);
    return 0;
}
any expression_statement :: accept(ast::visitor* vt) {
    vt->visit_expression_statement(this);
    return 0;
}
any declaration_statement :: accept(ast::visitor* vt) {
    vt->visit_declaration_statement(this);
    return 0;
}
any print_statement :: accept(ast::visitor* vt) {
    vt->visit_print_statement(this);
    return 0;
}
any input_statement :: accept(ast::visitor* vt) {
    vt->visit_input_statement(this);
    return 0;
}
any return_statement :: accept(ast::visitor* vt) {
    vt->visit_return_statement(this);
    return 0;
}
any function_declaration_statement :: accept(ast::visitor* vt) {
    vt->visit_function_declaration_statement(this);
    return 0;
}

any binary_expression :: accept(visitor* vt) {
    return  vt->visit_binary_expression(this);
}
any unary_expression ::  accept(visitor* vt) {
    return  vt->visit_unary_expression(this);
}
any literal_expression :: accept(visitor* vt) {
    return  vt->visit_literal_expression(this);
}
any logical_expression:: accept(visitor* vt) {
    return  vt->visit_logical_expression(this);
}
any variable_literal_expression:: accept(visitor* vt) {
    return  vt->visit_variable_literal_expression(this);
}
any class_declaration_statement:: accept(visitor* vt) {
    return  0;
}
any function_call_expression:: accept(visitor* vt) {
    return  vt->visit_function_call_expression(this);
}


