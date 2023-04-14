#include <iostream>
#include<iomanip>
#include "ast.h"
#include "token.h"
#include "environment.h"


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

variable_literal_expression:: variable_literal_expression(token  &variable_name): variable_name(variable_name) {}
any variable_literal_expression:: evaluate(){

       
        return this->variable_name;
        //we dont whether the function which called this eval wants lvalue or rvalue. hence let the upper function use the identifier name the way it wants (for rvalue, or lvalue)


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


any binary_expression:: evaluate(){

        any left = this->left->evaluate();
        any right = this->right->evaluate();
        auto env = environment:: get_environment();
        right = (right.type() == typeid(token)? env->get_variable(any_cast<token>(right)): right);

        if(this->optr.type == EQUAL){

            env->set_variable(any_cast<token>(left),right);
            return right;

        }
        left = (left.type() == typeid(token)? env->get_variable(any_cast<token>(left)): left);
        //above step are the value extraction steps. just in case the variables are undefined, error will be thrown

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
        }
}


print_statement:: print_statement(expression *exp): exp(exp) {}; 
expression_statement:: expression_statement(expression *exp): exp(exp) {}; 
declaration_statement:: declaration_statement(expression *exp,token variable_name): exp(exp),variable_name(variable_name) {};
conditional_statement:: conditional_statement(expression *expr, statement* if_statements, statement* else_statements): expr(expr), if_statements(if_statements),else_statements(else_statements) {};
block_statement:: block_statement(vector<statement*> &statements): statements(statements) {};
while_statement:: while_statement(expression* expr, statement* statements): expr(expr), statements(statements) {};

bool get_truth_value(any retval){

    //TODO: create an utils file and put this function in it
    bool val = false;

    if(retval.type() == typeid(bool)) val = any_cast<bool>(retval);
    if(retval.type() == typeid(int)) val = any_cast<int>(retval)>0;
    if(retval.type() == typeid(double)) val = any_cast<double>(retval)>0;
    if(retval.type() == typeid(string)) val = any_cast<string>(retval).size()>0;
    if(retval.type() == typeid(token)){
        
        auto env = environment::get_environment();
        
        val = get_truth_value(env->get_variable(any_cast<token>(retval)));

    }

    return val;
 

}
void while_statement:: execute(){

    while(get_truth_value(this->expr->evaluate())){


            this->statements->execute();
        
    }
    
}
void conditional_statement:: execute(){

    bool val = get_truth_value(this->expr->evaluate());

    if(val){

        this->if_statements->execute();
    }
    else
    {
        
        this->else_statements->execute();

    }

     
}

void block_statement:: execute(){


    for(auto at: this->statements){

        at->execute();
    }
}
    

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
        else if(val.type() == typeid(bool)){

            cout<<any_cast<bool>(val)<<endl;
        }
        else if(val.type() == typeid(token)){ //edge case: expressions containing only a single variable will return a token (variable_name, varaible_literal_expression
            
            auto env = environment:: get_environment();
            any value = env->get_variable(any_cast<token>(val));

            if(value.type() == typeid(string)){
                cout<<any_cast<string>(value)<<endl;

            }
            else if(value.type() == typeid(double)){
                cout<<any_cast<double>(value)<<endl;


            }
            else if(value.type() == typeid(int)){
                cout<<any_cast<int>(value)<<endl;


            }
            else if(value.type() == typeid(bool)){

                cout<<any_cast<bool>(value)<<endl;
            }
        }

};
void expression_statement :: execute() {

     this->exp->evaluate();

};
void declaration_statement:: execute() {

     
     auto env = environment:: get_environment();
     env->add_variable(this->variable_name,NULL); //this will fail if variable already exists with this name
     any value = this->exp->evaluate();
    };



                
            
