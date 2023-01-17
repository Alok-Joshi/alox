#include "ast.h"


using namespace ast;




binary_expression:: binary_expression(expression *left, token &optr, expression *right){
                    
                    this->left = left;
                    this->optr = optr;
                    this->right = right;
}


void binary_expression:: print_expression(){
                
                cout<<"expr( ";
                this->left->print_expression();
                cout<<", "<<
                this->optr->print_token()
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
                this->optr->print_token();
                cout<<", ";
                this->right->print_expression();
                cout<<" )";

}

literal_expression:: literal_expression(token &literal){

                this->literal = literal;
}

void literal_expression:: print_expression(){

                cout<<"expr( ";
                this->literal->print_token();
                cout<<") ";

}





                
            
