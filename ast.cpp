#include <functional>
#include <iostream>
#include<iomanip>
#include "ast.h"
#include "token.h"
#include "environment.h"
#include "callable.h"


using namespace ast;
using namespace std;
using namespace tok;




binary_expression:: binary_expression(expression *left, token& optr, expression *right){
                    
                    this->left = left;
                    this->optr = optr;
                    this->right = right;
}

logical_expression:: logical_expression(expression *left, token&optr, expression *right): binary_expression(left,optr,right) {};


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
            case TRUE:
                return 1;
            case FALSE:
                return 0;
        }
}


print_statement:: print_statement(expression *exp): exp(exp) {}; 
expression_statement:: expression_statement(expression *exp): exp(exp) {}; 
declaration_statement:: declaration_statement(expression *exp,token variable_name): exp(exp),variable_name(variable_name) {};
conditional_statement:: conditional_statement(expression *expr, statement* if_statements, statement* else_statements): expr(expr), if_statements(if_statements),else_statements(else_statements) {};
block_statement:: block_statement(vector<statement*> &statements): statements(statements) {};
while_statement:: while_statement(expression* expr, statement* statements): expr(expr), statements(statements) {};
for_statement:: for_statement(statement *part1, expression *part2, expression* part3,statement* statements): part1(part1), part2(part2), part3(part3), statements(statements) {};
function_call_expression:: function_call_expression(expression* function_name,vector<expression*> &arguments): function_name(function_name), arguments(arguments) {};
function_declaration_statement::function_declaration_statement(tok::token function_name, std:: vector<tok::token> &parameters, statement* block): function_name(function_name), parameters(parameters), block(block) {};
return_statement:: return_statement(expression *return_exp): return_exp(return_exp) {};
class_declaration_statement:: class_declaration_statement(tok:: token variable_name, std:: vector<statement*> methods): variable_name(variable_name), methods(methods) {}




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

any get_value(any val){
    //this is just to make sure that the value is bool, int , string and never token

    if(val.type() == typeid(token)){
    
        auto env = environment::get_environment();

        return env->get_variable(any_cast<token>(val));


    }
    else return val;


}

any return_statement:: execute(){

    auto rtobj = return_object(get_value(this->return_exp->evaluate()));
    return rtobj;
}
any function_declaration_statement:: execute(){

        auto env = environment:: get_environment();
        callable* fn_obj = new callable(this->parameters, this->block);
        env->add_variable(this->function_name,fn_obj);
        return 0;
}

any function_call_expression:: evaluate(){

        
    vector<any> args;

    for(int i = 0; i<this->arguments.size(); i++){
        
        args.push_back(get_value(arguments[i]->evaluate()));
    }


    auto env = environment::get_environment();
    auto fn = any_cast<callable*>(env->get_variable(any_cast<token>(this->function_name->evaluate())));
    auto ret_obj =  fn->call(args);

    return any_cast<return_object>(ret_obj).value;

}

void function_call_expression:: print_expression(){};

any for_statement:: execute(){

    
    auto env = environment:: get_environment();
    env->push_scope();
    part1->execute(); //TODO: Could be null, need to do null checking
    while(get_truth_value(part2->evaluate())){
        
        any val = this->statements->execute();
        if(val.type() == typeid(return_statement)){

            env->pop_scope();
            return val;

        }
        part3->evaluate();


    }
    env->pop_scope();
    return 0;


}
any while_statement:: execute(){

    auto env = environment::get_environment();
    env->push_scope();
    while(get_truth_value(this->expr->evaluate())){


            any val = this->statements->execute();
            if(val.type() == typeid(return_object)){

                env->pop_scope();
                return val;
            }

        
    }
    env->pop_scope();
    return 0;
    
}
any conditional_statement:: execute(){

    bool val = get_truth_value(this->expr->evaluate());
    auto env = environment:: get_environment();
    env->push_scope();

    if(val){

        any stat_val = this->if_statements->execute();
        if(stat_val.type() == typeid(return_object)){

            env->pop_scope();
            return stat_val;

        }
    }
    else
    {
        
        any stat_val = this->else_statements->execute();

        if(stat_val.type() == typeid(return_object)){

            env->pop_scope();
            return stat_val;


        }

    }

    env->pop_scope();
    return 0;

     
}
any logical_expression:: evaluate(){



    any left  = this->left->evaluate();

    //short circuiting
    
    if(this->optr.type == OR){

        bool left_truth_value = get_truth_value(left);
        if(left_truth_value){
            
            return left_truth_value;
        }
    }

    else if(this->optr.type == AND){

        bool left_truth_value = get_truth_value(left);

        if(left_truth_value){

            return left_truth_value;
        }


    }




    any right = this->right->evaluate();
    if(left.type() == right.type()){
        

        switch(this->optr.type){

            
            case OR:
                return get_truth_value(left) || get_truth_value(right);
            case AND:
                return get_truth_value(left) && get_truth_value(right);

            default:
                return 0;

    }
    }


        return 0;

}

void logical_expression:: print_expression(){

                cout<<"expr( ";
                this->left->print_expression();
                cout<<", ";
                this->optr.print_token();
                cout<<", ";
                this->right->print_expression();
                cout<<" )";

 


}
any block_statement:: execute(){


    auto env = environment:: get_environment();
    env->push_scope();
    for(auto at: this->statements){

        any val = at->execute();

        if(val.type() == typeid(return_object)){


            env->pop_scope();
            return val;
        }
    }

    env->pop_scope();

    return 0;
}
    

any print_statement:: execute() {

        any val = get_value(this->exp->evaluate());
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


       return 0;

}
any expression_statement :: execute() {

    return this->exp->evaluate();


}


any declaration_statement:: execute() {

     
     auto env = environment:: get_environment();
     env->add_variable(this->variable_name,NULL); //this will fail if variable already exists with this name
     any value = this->exp->evaluate();
     return 0;
}



any class_declaration_statement:: execute(){
    //TODO: Yet to define representation of functions (similar to that of callable)

}



                
            
