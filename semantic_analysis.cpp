#include "ast.h"
#include "environment.h"


using namespace std;
using namespace ast;




semantic_analyser:: semantic_analyser(vector<statement*> ast): ast(ast) {

            this->symtab = new symbol_table();

};



bool semantic_analyser:: analyse_program() {
    

    this->env->();
    bool scope_val = true;
    for(auto &stmt: ast){

        scope_val = scope_val && check_scope(stmt);
    }

    this->symtab->push_scope();

    return scope_val;


}


bool semantic_analyser:: check_scope(statement* stmt){

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
            
            this->symtab->add_entry(dec_stmt->variable_name);
        
        }

    }

    else if(typeid(*stmt) == typeid(function_declaration_statement)){

        auto fd_stmt = static_cast<function_declaration_statement*>(stmt);

        if(this->symtab->is_redeclaration(fd_stmt->function_name)){
            
            throw "ERROR: Function Redeclared";

        }
        else
        {
            this->symtab->add_entry(fd_stmt->function_name);
            this->symtab->start_scope(); 
            //function scope starts here, we put the parameter names of the function in this newly created scope

            for(auto &parameter: fd_stmt->parameters){
                
                this->symtab->add_entry(parameter);
            }

            //now check the block

            bool block_scope_check = block_resolver(static_cast<block_statement*>(fd_stmt->block));

            this->symtab->end_scope();
            return block_scope_check;


        }
        

    }

    else if(typeid(*stmt) == typeid(for_statement)){

        auto for_stmt = static_cast<for_statement*>(stmt);

        this->symtab->start_scope();

        bool part1_check = check_scope(for_stmt->part1);
        bool part2_check = check_scope(for_stmt->part2);
        bool part3_check = check_scope(for_stmt->part3);

        if(typeid(*(for_stmt->statements)) == typeid(block_statement)){

            auto for_body = static_cast<block_statement*>(for_stmt->statements);
            this->symtab->end_scope();
            return part1_check && part2_check && part3_check && block_resolver(for_body);
        }
        else
        {
            bool for_body_check = check_scope(for_stmt->statements);
            this->symtab->end_scope();
            return part1_check && part2_check && part3_check && for_body_check;


        }



    }


    else if(typeid(*stmt) == typeid(expression_statement)){

        
        auto exp_stmt = static_cast<expression_statement*>(stmt);
        bool exp_scope_check = check_scope(exp_stmt->exp);

        return exp_scope_check;

    }

    else if(typeid(*stmt) == typeid(print_statement)){

        auto print_stmt = static_cast<print_statement*>(stmt);
        bool exp_scope_check = check_scope(print_stmt->exp);
        return exp_scope_check;

    }
    else if(typeid(*stmt) == typeid(return_statement)){

        auto return_stmt = static_cast<return_statement*>(stmt);
        bool exp_scope_check = check_scope(return_stmt->return_exp);
        return exp_scope_check;

    }



    else if(typeid(*stmt) == typeid(conditional_statement)){

        

        auto cond_stmt = static_cast<conditional_statement*>(stmt);
        bool exp_scope_check = check_scope(cond_stmt->expr);
        bool if_stmt_scope_check = check_scope(cond_stmt->if_statements);
        bool else_statements_scope_check = check_scope(cond_stmt->else_statements);

        return exp_scope_check && if_stmt_scope_check && else_statements_scope_check;


    }

    else if(typeid(*stmt) == typeid(block_statement)){

            
        this->symtab->push_scope();

        auto block_stmt = static_cast<block_statement*>(stmt);
        bool final_result = block_resolver(block_stmt);

        this->symtab->pop_scope();
        return final_result;


    }

    else if(typeid(*stmt) == typeid(while_statement)){

         
        auto while_stmt = static_cast<while_statement*>(stmt);
        bool expr_check = check_scope(while_stmt->expr);
        bool body_check = check_scope(while_stmt->statements);

        return expr_check && body_check;

    }

    }

bool semantic_analyser:: block_resolver(block_statement* block){

        bool final_result = true;
        for(auto stmt: block->statements){
        

            final_result = final_result && check_scope(stmt);

        }


        return final_result;



}

bool semantic_analyser:: check_scope(expression* exp){

            
      if(typeid(*exp) == typeid(variable_literal_expression)){

                
                variable_literal_expression *varexp =static_cast<variable_literal_expression*>(exp);
                if(symtab->resolve_variable(varexp->variable_name)){

                    return true;
                }
                else
                {
                    throw "unknown variable "; //TODO: need to do this
                }

      }

      else if(typeid(*exp) == typeid(logical_expression) || typeid(*exp) == typeid(binary_expression)){

                
            binary_expression * binexp = static_cast<binary_expression*>(exp);
            return check_scope(binexp->left) && check_scope(binexp->right);

      }

      else if(typeid(*exp) == typeid(unary_expression)) {
                
            unary_expression *unexp = static_cast<unary_expression*>(exp);
            return check_scope(unexp->right);

      }

      else if(typeid(*exp) == typeid(function_call_expression)){

        
            
          function_call_expression *fun_exp = static_cast<function_call_expression*>(exp);

          if(symtab->resolve_identifier(fun_exp->function_name)){

                return true;

          }
          else{

              throw "Unknown function name ";


          }


      }

      else {

          return true; //must be literal value expression
      }
      


}


//Semantic analysis: 

// type checking
// function call argument checking
// 
