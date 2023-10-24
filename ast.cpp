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
expression:: expression(int line_number): line_number(line_number) {};
statement:: statement(int line_number): line_number(line_number) {};
binary_expression:: binary_expression(expression *left, token& optr, expression *right,int line_number): left(left), optr(optr), right(right), expression(line_number) {}
unary_expression:: unary_expression(token &optr,expression *right,int line_number): optr(optr),right(right),expression(line_number) {}
logical_expression:: logical_expression(expression *left, token&optr, expression *right,int line_number): binary_expression(left,optr,right,line_number) {}
literal_expression:: literal_expression(token &literal,int line_number): literal(literal), expression(line_number){}
variable_literal_expression:: variable_literal_expression(token  &variable_name,int line_number): variable_name(variable_name), expression(line_number) {}

void binary_expression:: print_expression(){
                
                cout<<"expr( ";
                this->left->print_expression();
                cout<<", ";
                this->optr.print_token();
                cout<<", ";
                this->right->print_expression();
                cout<<" )";

}

void unary_expression:: print_expression(){
                
                cout<<"expr( ";
                this->optr.print_token();
                cout<<", ";
                this->right->print_expression();
                cout<<" )";

}

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
                
//ACCEPT implementation
void conditional_statement:: accept(ast::visitor* vt){ vt->visit_conditional_statement(this); }
void block_statement :: accept(ast::visitor* vt){vt->visit_block_statement(this); }
void while_statement :: accept(ast::visitor* vt){vt->visit_while_statement(this); }
void for_statement :: accept(ast::visitor* vt){ vt->visit_for_statement(this); }
void expression_statement :: accept(ast::visitor* vt){ vt->visit_expression_statement(this); }
void declaration_statement :: accept(ast::visitor* vt){ vt->visit_declaration_statement(this); }
void print_statement :: accept(ast::visitor* vt){ vt->visit_print_statement(this); }
void input_statement :: accept(ast::visitor* vt){ vt->visit_input_statement(this);}
void return_statement :: accept(ast::visitor* vt){ vt->visit_return_statement(this); }
void function_declaration_statement :: accept(ast::visitor* vt){ vt->visit_function_declaration_statement(this); }

void binary_expression :: accept(visitor* vt){ vt->visit_binary_expression(this); }
void unary_expression ::  accept(visitor* vt){ vt->visit_unary_expression(this); }
void literal_expression :: accept(visitor* vt){ vt->visit_literal_expression(this); }
void logical_expression:: accept(visitor* vt){ vt->visit_logical_expression(this); }
void variable_literal_expression:: accept(visitor* vt){ vt->visit_variable_literal_expression(this); } 
void function_call_expression:: accept(visitor* vt){ vt->visit_function_call_expression(this); }


