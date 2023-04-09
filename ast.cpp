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


any binary_expression:: evaluate(){

        any left = this->left->evaluate();
        any right = this->right->evaluate();
        if(left.type() == right.type()){

            if(left.type() == typeid(string)){

                switch(this->optr.type){

                    case PLUS:
                        return any_cast<string>(left) + any_cast<string>(right);
                    default:
                        throw "illegal operands";
                }


            }
        
            else{
                switch(this->optr.type){

                    case PLUS:
                        return any_cast<double>(left) + any_cast<double>(right);
                    case MINUS:
                        return any_cast<double>(left) - any_cast<double>(right);
                    case STAR:
                        return any_cast<double>(left) * any_cast<double>(right);
                    case SLASH:
                        return any_cast<double>(left) / any_cast<double>(right);
                    case BANG_EQUAL:
                        return any_cast<double>(left) != any_cast<double>(right);
                    case EQUAL_EQUAL:
                        return any_cast<double>(left) == any_cast<double>(right);
                    case GREATER:
                        return any_cast<double>(left) > any_cast<double>(right);
                    case GREATER_EQUAL:
                        return any_cast<double>(left) >= any_cast<double>(right);
                    case LESS:
                        return any_cast<double>(left) < any_cast<double>(right);
                    case LESS_EQUAL:
                        return any_cast<double>(left) <= any_cast<double>(right);
                    default: 
                         return 0;

        }
        }
        }
        else
        {
            
            throw "illegal operand types ";
        
        }



}

any unary_expression:: evaluate(){

        any right = this->right->evaluate();

        if(right.type() == typeid(double)){

        switch(this->optr.type){

            case MINUS:
                return (-any_cast<double>(right));
            case BANG:
                return (!any_cast<double>(right));
            
            default:
                return 0;
        }
        }
        else
        {
            throw "illegal operand type";
        }



}

any literal_expression:: evaluate(){


        switch(this->literal.type){

            case NUMBER:
                return any_cast<double>(this->literal.int_literal_value);
            case STRING:
                return any_cast<string>(this->literal.string_literal_value);
            case IDENTIFIER:
                //TODO: yet to implement. will use a lookup table
                return 0;
            default:
                return 0;

        }
}


print_statement:: print_statement(expression *exp): exp(exp) {}; 
expression_statement:: expression_statement(expression *exp): exp(exp) {}; 
declaration_statement:: declaration_statement(expression *exp): exp(exp) {}; 


void print_statement:: execute() {

        any val = this->exp->evaluate();
        if(val.type() == typeid(string)){

            cout<<any_cast<string>(val)<<endl;

        }
        else if(val.type() == typeid(double)){


            cout<<any_cast<double>(val)<<endl;

        }
        else if(val.type() == typeid(int)){

            cout<<any_cast<int>(val)<<endl;
        }

};
void expression_statement :: execute() {


};
void declaration_statement:: execute() {

    



};


                
            
