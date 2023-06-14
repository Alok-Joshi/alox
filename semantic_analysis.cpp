#include "ast.h"
#include "environment.h"
#include "token.h"
#include <any>

using namespace std;
using namespace ast;
using namespace tok;




semantic_analyser:: semantic_analyser(vector<statement*> ast): ast(ast) {

            this->symtab = new symbol_table();

};



bool semantic_analyser:: analyse_program() {
    

    this->symtab->start_scope();
    bool scope_val = true;
    for(auto &stmt: ast){

        scope_val = scope_val && analyse_statement(stmt);
    }

    this->symtab->end_scope();
    return scope_val;


}


bool semantic_analyser:: analyse_statement(statement* stmt){

    //if else statements for each statement type. Make sure the for appropriate statements, (for , while, block, function) open new block, and insert into environment for declaration
    //also, resolve the issue of block and stmt conflict for while
    
    if(typeid(*stmt) == typeid(declaration_statement)){

        auto dec_stmt = static_cast<declaration_statement*>(stmt);

        if(this->symtab->is_redeclaration(dec_stmt->variable_name)){
                //first check if this variable has been declared in the same scope block

            throw "ERROR: Variable redeclared"; //improve the error later on
                

        }
        else
        {
            
            //we insert the variable and its type. The checking of whether a proper variable is being assigned is done by the analyse_expression itself
            symbol_table_entry symb_entry{dec_stmt->variable_type};
            this->symtab->add_entry(dec_stmt->variable_name,symb_entry);

            auto dec_exp_result = analyse_expression(dec_stmt->exp);

            return dec_exp_result.first;

        
        }

    }

    else if(typeid(*stmt) == typeid(function_declaration_statement)){

        auto fd_stmt = static_cast<function_declaration_statement*>(stmt);

        if(this->symtab->is_redeclaration(fd_stmt->function_name)){
            
            throw "ERROR: Function Redeclared";

        }
        else
        {

           symbol_table_entry symbtab_entry{FUNCTION_TYPE,fd_stmt->parameters,fd_stmt->return_type};

            this->symtab->add_entry(fd_stmt->function_name,symbtab_entry); 
            this->symtab->start_scope(fd_stmt->function_name); 

            for(auto &parameter: fd_stmt->parameters){
                

                symbol_table_entry symb_entry{parameter.second};
                this->symtab->add_entry(parameter.first, symb_entry);
            }

            //now check the block

            bool block_scope_check = block_resolver(static_cast<block_statement*>(fd_stmt->block));

            bool is_function_scope = true;
            this->symtab->end_scope(is_function_scope);
            return block_scope_check;


        }
        

    }

    else if(typeid(*stmt) == typeid(for_statement)){

        auto for_stmt = static_cast<for_statement*>(stmt);

        this->symtab->start_scope();

        bool part1_check = analyse_statement(for_stmt->part1);
        auto part2_check = analyse_expression(for_stmt->part2);
        auto part3_check = analyse_expression(for_stmt->part3);


        if(typeid(*(for_stmt->statements)) == typeid(block_statement)){

            auto for_body = static_cast<block_statement*>(for_stmt->statements);
            bool body_check = block_resolver(static_cast<block_statement*>(for_stmt->statements));

            this->symtab->end_scope();
            return part1_check && part2_check.first && part3_check.first && body_check;
        }
        else
        {
            bool for_body_check = analyse_statement(for_stmt->statements);
            this->symtab->end_scope();
            return part1_check && part2_check.first && part3_check.first && for_body_check;


        }



    }


    else if(typeid(*stmt) == typeid(expression_statement)){

        
        auto exp_stmt = static_cast<expression_statement*>(stmt);
        auto exp_check = analyse_expression(exp_stmt->exp);

        return exp_check.first;

    }

    else if(typeid(*stmt) == typeid(print_statement)){

        auto print_stmt = static_cast<print_statement*>(stmt);
        auto exp_check = analyse_expression(print_stmt->exp);
        return exp_check.first;

    }
    else if(typeid(*stmt) == typeid(return_statement)){

        auto return_stmt = static_cast<return_statement*>(stmt);
        auto return_check = analyse_expression(return_stmt->return_exp);


        token current_function_name = this->symtab->get_current_function();
        symbol_table_entry symtab_entry = this->symtab->get_entry(current_function_name); //get the current entry
        token_type fnt_return_type = symtab_entry.return_type;


        if(fnt_return_type != return_check.second){

            throw "Function return type mismatch ";

        }
        
        return return_check.first;

    }


    else if(typeid(*stmt) == typeid(conditional_statement)){

        

        auto cond_stmt = static_cast<conditional_statement*>(stmt);
        auto exp_scope_check = analyse_expression(cond_stmt->expr);
        bool if_stmt_scope_check = analyse_statement(cond_stmt->if_statements);
        bool else_statements_scope_check = analyse_statement(cond_stmt->else_statements);

        return exp_scope_check.first && if_stmt_scope_check && else_statements_scope_check;


    }

    else if(typeid(*stmt) == typeid(block_statement)){

            
        this->symtab->start_scope();

        auto block_stmt = static_cast<block_statement*>(stmt);
        bool final_result = block_resolver(block_stmt);

        this->symtab->end_scope();
        return final_result;


    }

    else if(typeid(*stmt) == typeid(input_statement)){


        input_statement* inp_stmt = static_cast<input_statement*>(stmt);
        bool variable_resolved = this->symtab->resolve_identifier(inp_stmt->input_reciever_variable);
        if(!variable_resolved){ //Part I: Resolve variable
    
            throw "unknown variable";

        }


        symbol_table_entry inp_variable_entry = this->symtab->get_entry(inp_stmt->input_reciever_variable);

        if(inp_variable_entry.symbol_type != inp_stmt->input_type){

            throw "Invalid input variable type at ";


        }


        return true;






    }
    else if(typeid(*stmt) == typeid(while_statement)){

         
        auto while_stmt = static_cast<while_statement*>(stmt);
        auto expr_check = analyse_expression(while_stmt->expr);
        auto body_check = analyse_statement(while_stmt->statements);

        return expr_check.first && body_check;

    }

    }

bool semantic_analyser:: block_resolver(block_statement* block){

        bool final_result = true;
        for(auto stmt: block->statements){
        

            final_result = final_result && analyse_statement(stmt);

        }


        return final_result;


}

pair<bool,token_type> semantic_analyser:: analyse_expression(expression* exp){

            

        if(typeid(*exp) == typeid(literal_expression)){


            literal_expression *litexp =static_cast<literal_expression*>(exp);
            litexp->expression_type = litexp->literal.type;
            auto return_obj = make_pair(true, litexp->literal.type);

            return return_obj;



        }
        else if(typeid(*exp) == typeid(variable_literal_expression)){

                
                variable_literal_expression *varexp =static_cast<variable_literal_expression*>(exp);
                bool variable_resolved = symtab->resolve_identifier(varexp->variable_name); //Part I: Variable resolution 

                if(!variable_resolved){

                    
                    throw "Unknown variable";

                }


                symbol_table_entry symb_entry = this->symtab->get_entry(varexp->variable_name); //Part II: Type infering
                varexp->expression_type = symb_entry.symbol_type;

                auto node_result = make_pair(true,symb_entry.symbol_type);
                return node_result;


                

      }

        else if(typeid(*exp) == typeid(logical_expression) || typeid(*exp) == typeid(binary_expression)){

                
            binary_expression * binexp = static_cast<binary_expression*>(exp);


            if(binexp->optr.type == EQUAL){
                
                auto right_expression_result = analyse_expression(binexp->right);
                token lvalue = static_cast<variable_literal_expression*>(binexp->left)->variable_name;
                bool variable_resolved = symtab->resolve_identifier(lvalue);

                if(!variable_resolved){

                    throw "Unknown variable ";

                }

                symbol_table_entry symtab_entry = symtab->get_entry(lvalue);


                if(symtab_entry.symbol_type != right_expression_result.second){

                        //implies wrong type of value being assigned to the variable

                        throw "Type Mismatch";


                }


                return make_pair(true,symtab_entry.symbol_type);


            }
            else
            {

                auto left_result = analyse_expression(binexp->left);
                auto right_result = analyse_expression(binexp->right);

                if(left_result.second == right_result.second) {

                    binexp->expression_type = left_result.second;
                    return make_pair(true,left_result.second);

                }
                else
                {

                    throw "Error: Type Mismatch in expression";

                }
            }

      }

        else if(typeid(*exp) == typeid(unary_expression)) {
                
            unary_expression *unexp = static_cast<unary_expression*>(exp);
            auto right_result = analyse_expression(unexp->right);
            unexp->expression_type = right_result.second;
            return right_result;

      }

        else if(typeid(*exp) == typeid(function_call_expression)){

        
            
          function_call_expression *fun_exp = static_cast<function_call_expression*>(exp);
          bool function_name_resolved = symtab->resolve_identifier(fun_exp->function_name);

          if(!function_name_resolved){


              throw "Unknown function name ";


          }

          else
          {
            
              symbol_table_entry symtab_entry = this->symtab->get_entry(fun_exp->function_name);//will return the arg_count
              vector<pair<token,token_type>> parameters = symtab_entry.parameters;
              token_type function_return_type = symtab_entry.return_type;
              
              
              //check argument count matches parameter count
              if(parameters.size() != fun_exp->arguments.size()){


                    throw "ERROR: Invalid number of arguments for function call ";

              }

              //if the function return type is Void, then it cannot be used in an expression
            
              if(function_return_type == tok::VOID_TYPE){

                  throw "ERROR: Void function cannot return value";

              }


              //now check each argument given to the function call. arg type should match the parameter of function

              bool result = true;

              for(int i = 0; i<fun_exp->arguments.size(); i++){
                    
                  
                  auto exp_result = analyse_expression(fun_exp->arguments[i]);
                  if(exp_result.second != parameters[i].second){

                      throw "Argument type mismatch";

                  }
                  result = result | exp_result.first;

              }

              return make_pair(result,function_return_type);


          }


      }



}


//Semantic analysis: 

// type checking
// function call argument checking
// unneccessary return statements 
