#include <iostream>
#include<iomanip>
#include "ast.h"
#include "token.h"


using namespace ast;
using namespace std;
using namespace tok;




binary_expression:: binary_expression(expression *left, token& optr, expression *right){
                    
                    this->left = left;
                    this->optr = optr;
                    this->right = right;
}


void binary_expression:: print_expression(){
                
                cout<<"expr( ";
                this->left->print_expression();
                cout<<", ";
                this->optr.print_token();
                cout<<", ";
                this->right->print_expression();
                cout<<" )";

}


unary_expression:: unary_expression(token &optr,expression *right){

                this->optr = optr;
                this->right = right;

}

void unary_expression:: print_expression(){
                
                cout<<"expr( ";
                this->optr.print_token();
                cout<<", ";
                this->right->print_expression();
                cout<<" )";

}

literal_expression:: literal_expression(token &literal){

                this->literal = literal;
}

void literal_expression:: print_expression(){

                cout<<"expr( ";
                this->literal.print_token();
                cout<<") ";

}


double binary_expression:: evaluate(){

        double left = this->left->evaluate();
        double right = this->right->evaluate();

        switch(this->optr.type){

            case PLUS:
                return left + right;
            case MINUS:
                return left - right;
            case STAR:
                return left*right;
            case SLASH:
                return left/right;
            case BANG_EQUAL:
                 return left != right;
            case EQUAL_EQUAL:
                 return left == right;
            case GREATER:
                 return left > right;
            case GREATER_EQUAL:
                 return left >= right;
            case LESS:
                 return left < right;
            case LESS_EQUAL:
                 return left <= right;
            default: 
                 return 0;

        }

}

double unary_expression:: evaluate(){

        double right = this->right->evaluate();

        switch(this->optr.type){

            case MINUS:
                return (-right);
            case BANG:
                return (!right);
            
            default:
                return 0;
        }



}

double literal_expression:: evaluate(){


        switch(this->literal.type){

            case NUMBER:
                return double(this->literal.int_literal_value);
            case STRING:
                return 0; //TODO: NEED to resolve string literal evaluation. Use runtimetype evalaution in upper nodes (get left, get right, check if both types are double or both types are string, then do the evaluation
            default:
                return 0;

        }
}


print_statement:: print_statement(expression *exp): exp(exp) {}; 
expression_statement:: expression_statement(expression *exp): exp(exp) {}; 
declaration_statement:: declaration_statement(expression *exp): exp(exp) {}; 


void print_statement:: execute() {

        double val = this->exp->evaluate();
        cout<<val<<endl;

};
void expression_statement :: execute() {};
void declaration_statement:: execute() {};


                
            
