#include <functional>
#include <iostream>
#include<iomanip>
#include "ast.h"
#include "token.h"
#include "environment.h"


using namespace ast;
using namespace std;
using namespace tok;




expression:: expression(int line_number): line_number(line_number) {};
statement:: statement(int line_number): line_number(line_number) {};

binary_expression:: binary_expression(expression *left, token& optr, expression *right,int line_number): left(left), optr(optr), right(right), expression(line_number) {}

logical_expression:: logical_expression(expression *left, token&optr, expression *right,int line_number): binary_expression(left,optr,right,line_number) {}


void binary_expression:: print_expression(){
                
                cout<<"expr( ";
                this->left->print_expression();
                cout<<", ";
                this->optr.print_token();
                cout<<", ";
                this->right->print_expression();
                cout<<" )";

}


unary_expression:: unary_expression(token &optr,expression *right,int line_number): optr(optr),right(right),expression(line_number) {}


void unary_expression:: print_expression(){
                
                cout<<"expr( ";
                this->optr.print_token();
                cout<<", ";
                this->right->print_expression();
                cout<<" )";

}

literal_expression:: literal_expression(token &literal,int line_number): literal(literal), expression(line_number){}

variable_literal_expression:: variable_literal_expression(token  &variable_name,int line_number): variable_name(variable_name), expression(line_number) {}


token variable_literal_expression:: get_variable_name(){

        return this->variable_name;

}


void variable_literal_expression:: print_expression(){

        cout<<"expr( ";
        this->variable_name.print_token();
        cout<<" )";
}

void literal_expression:: print_expression(){

                cout<<"expr( ";
                this->literal.print_token();
                cout<<") ";

}





//constructors 

print_statement:: print_statement(expression *exp,int line_number): exp(exp),statement(line_number) {}; 
expression_statement:: expression_statement(expression *exp,int line_number): statement(line_number),exp(exp) {}; 
declaration_statement:: declaration_statement(expression *exp,token variable_name,token_type variable_type,int line_number): statement(line_number), exp(exp),variable_name(variable_name), variable_type(variable_type) {};
conditional_statement:: conditional_statement(expression *expr, statement* if_statements, statement* else_statements,int line_number): statement(line_number),expr(expr), if_statements(if_statements),else_statements(else_statements) {};
block_statement:: block_statement(vector<statement*> &statements,int line_number): statement(line_number),statements(statements) {};
while_statement:: while_statement(expression* expr, statement* statements,int line_number): statement(line_number),expr(expr), statements(statements) {};
for_statement:: for_statement(statement *part1, expression *part2, expression* part3,statement* statements,int line_number): statement(line_number),part1(part1), part2(part2), part3(part3), statements(statements) {};
function_call_expression:: function_call_expression(token function_name,vector<expression*> &arguments,int line_number): expression(line_number),function_name(function_name), arguments(arguments) {};
function_declaration_statement::function_declaration_statement(tok::token function_name, vector<pair<tok::token,tok::token_type>> &parameters, statement* block,token_type return_type,int line_number): statement(line_number),function_name(function_name), parameters(parameters), block(block), return_type(return_type) {};
return_statement:: return_statement(expression *return_exp,int line_number): statement(line_number),return_exp(return_exp) {};
class_declaration_statement:: class_declaration_statement(tok:: token variable_name, std:: vector<statement*> methods,int line_number): statement(line_number),variable_name(variable_name), methods(methods) {}

input_statement:: input_statement(token input_reciever_variable,token_type input_type,int line_number): statement(line_number),input_reciever_variable(input_reciever_variable), input_type(input_type) {};


void function_call_expression:: print_expression(){};

void logical_expression:: print_expression(){

                cout<<"expr( ";
                this->left->print_expression();
                cout<<", ";
                this->optr.print_token();
                cout<<", ";
                this->right->print_expression();
                cout<<" )";

 


}
                
            
