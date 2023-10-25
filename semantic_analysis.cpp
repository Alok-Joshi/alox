#include "ast.h"
#include "environment.h"
#include "token.h"
#include <any>
#include <string>
#include <utility>

using namespace std;
using namespace ast;
using namespace tok;




semantic_analyser:: semantic_analyser(vector<statement*> ast): ast(ast) {

            this->symtab = new symbol_table();

};



void semantic_analyser:: analyse_program() {
    this->symtab->start_scope();
    for(auto &stmt: ast){
        stmt->accept(this);
    }
    this->symtab->end_scope();
}


void semantic_analyser :: visit_declaration_statement(declaration_statement *dec_stmt){
        if(this->symtab->is_redeclaration(dec_stmt->variable_name)){
            string error = "ERROR at line " +to_string(dec_stmt->variable_name.line_number) + " : Redeclaration of identifier \"" + dec_stmt->variable_name.lexeme + "\"";
            error_stack.push_back(error);
        }
        else
        {
            symbol_table_entry symb_entry{dec_stmt->variable_type};
            this->symtab->add_entry(dec_stmt->variable_name,symb_entry);
            dec_stmt->exp->accept(this);
        }
}
void semantic_analyser:: visit_function_declaration_statement(function_declaration_statement* fd_stmt){
    if(this->symtab->is_redeclaration(fd_stmt->function_name)){
            string error = "ERROR at line " +to_string(fd_stmt->function_name.line_number) + " : Redeclaration of Function \"" + fd_stmt->function_name.lexeme + "\"";
            error_stack.push_back(error);

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

            bool block_scope_check = block_resolver(static_cast<block_statement*>(fd_stmt->block));
            bool is_function_scope = true;
            this->symtab->end_scope(is_function_scope);
            if(symbtab_entry.return_type != VOID_TYPE && !this->return_encountered){
                string error = "ERROR at line " + to_string(fd_stmt->function_name.line_number) + ": Function \"" + fd_stmt->function_name.lexeme + "\" has non void return type but no return statement ";
                error_stack.push_back(error);
            }
            this->return_encountered = false; //so that other functions can use it
        }
    
}

void semantic_analyser :: visit_for_statement(for_statement* for_stmt){
        this->symtab->start_scope();
        for_stmt->part1->accept(this);
        for_stmt->part2->accept(this);
        for_stmt->part3->accept(this);
        for_stmt->statements->accept(this);
        this->symtab->end_scope();
}


void semantic_analyser :: visit_expression_statement(expression_statement* exp_stmt){
         exp_stmt->exp->accept(this);
}

void semantic_analyser :: visit_print_statement(print_statement * print_stmt){
        print_stmt->exp->accept(this);
}

void semantic_analyser :: visit_return_statement(return_statement* return_stmt){
        return_encountered = true;
        return_stmt->return_exp->accept(this);
        token current_function_name = this->symtab->get_current_function();
        symbol_table_entry symtab_entry = this->symtab->get_entry(current_function_name); //get the current entry
        token_type fnt_return_type = symtab_entry.return_type;
        if(fnt_return_type != return_check.second){
            string error = "ERROR at line " + to_string(current_function_name.line_number) + " : Function \"" + current_function_name.lexeme + "\" has incorrect return type";
            error_stack.push_back(error);
        }
}
void semantic_analyser :: visit_conditional_statement(conditional_statement* cond_stmt){
        analyse_expression(cond_stmt->expr)->accept(this);
        cond_stmt->if_statements->accept(this);
        if(cond_stmt->else_statements != NULL){
            cond_stmt->else_statements->accept(this);
        }
}

void semantic_analyser :: visit_block_statement(block_statement* block_stmt){
        this->symtab->start_scope();
        auto block_stmt = static_cast<block_statement*>(stmt);
        bool final_result = block_resolver(block_stmt);
        this->symtab->end_scope();
        return final_result;
}

void semantic_analyser :: visit_input_statement(input_statement* input_stmt){
        bool variable_resolved = this->symtab->resolve_identifier(inp_stmt->input_reciever_variable);
        if(!variable_resolved){ //Part I: Resolve variable
            string error = "ERROR at line " + to_string(inp_stmt->input_reciever_variable.line_number) + " : Unknown Variable \"" + inp_stmt->input_reciever_variable.lexeme + "\"";
            error_stack.push_back(error);
        }
        symbol_table_entry inp_variable_entry = this->symtab->get_entry(inp_stmt->input_reciever_variable);
        if(inp_variable_entry.symbol_type != inp_stmt->input_type){
            string error = "ERROR at line " + to_string(inp_stmt->input_reciever_variable.line_number) + " : Type Mismatch:  Variable \"" + inp_stmt->input_reciever_variable.lexeme + "\"";
            error_stack.push_back(error);
        }
}
void semantic_analyser :: visit_while_statement(while_statement* while_stmt){
        while_stmt->expr->accept(this);
        while_stmt->statements->accept(this);
}

bool semantic_analyser:: block_resolver(block_statement* block){

        bool final_result = true;
        for(auto stmt: block->statements){
        

            final_result =  analyse_statement(stmt) && final_result;

        }
        return final_result;

}

string get_token_type(token_type tt){
    switch(tt){

        case STRING_TYPE:
            return "String";
        case NUMBER_TYPE:
            return "Number";
    }
}
pair<bool,token_type> semantic_analyser :: visit_literal_expression(literal_expression* litexp){
            litexp->expression_type = litexp->literal.type;
            auto return_obj = make_pair(true, litexp->literal.type);
            return return_obj;
}

pair<bool,token_type> semantic_analyser :: visit_variable_literal_expression(variable_literal_expression* varexp){
            bool variable_resolved = symtab->resolve_identifier(varexp->variable_name); //Part I: Variable resolution 
            if(!variable_resolved){
                string error = "ERROR at line " + to_string(varexp->variable_name.line_number) + " : Unknown Variable \"" + varexp->variable_name.lexeme + "\"";
                error_stack.push_back(error);
                return make_pair(false,ERROR);
            }
            else
            {
                symbol_table_entry symb_entry = this->symtab->get_entry(varexp->variable_name); //Part II: Type infering
                varexp->expression_type = symb_entry.symbol_type;
                auto node_result = make_pair(true,symb_entry.symbol_type);
                return node_result;
            }
}

pair<bool,token_type> semantic_analyser :: visit_binary_expression(binary_expression* binexp){
        auto left_result = binexp->left->accept(this);
        auto right_result = binexp->right->accept(this);
        if(left_result.second == right_result.second && left_result.second != ERROR){
            binexp->expression_type = left_result.second;
            return make_pair(true,left_result.second);
        }
        else if(left_result.second != ERROR && right_result.second!= ERROR) //implies left is of some other type and right is of some other type
        {
            
        string error = "ERROR at line " + to_string(binexp->line_number) + ": Invalid operand of type " + get_token_type(left_result.second) + " and " + get_token_type(right_result.second) + " to operator " + binexp->optr.lexeme;
        error_stack.push_back(error);


        }
            return make_pair(false,ERROR);
}

pair<bool,token_type> semantic_analyser :: visit_logical_expression(logical_expression* binexp){
        auto left_result = binexp->left->accept(this);
        auto right_result = binexp->right->accept(this);
        if(left_result.second == right_result.second && left_result.second != ERROR){
            binexp->expression_type = left_result.second;
            return make_pair(true,left_result.second);
        }
        else if(left_result.second != ERROR && right_result.second!= ERROR) //implies left is of some other type and right is of some other type
        {
            
        string error = "ERROR at line " + to_string(binexp->line_number) + ": Invalid operand of type " + get_token_type(left_result.second) + " and " + get_token_type(right_result.second) + " to operator " + binexp->optr.lexeme;
        error_stack.push_back(error);


        }
            return make_pair(false,ERROR);
}

pair<bool,token_type> semantic_analyser :: visit_unary_expression(unary_expression* unexp){
        auto right_result = unexp->right->accept(this);
        unexp->expression_type = right_result.second;
        return right_result;
}
pair<bool,token_type> semantic_analyser:: visit_function_call_expression(function_call_expression * fun_exp){ 
          bool function_name_resolved = symtab->resolve_identifier(fun_exp->function_name);
          if(!function_name_resolved){
            string error = "ERROR at line " + to_string(fun_exp->function_name.line_number) + " : Function \"" + fun_exp->function_name.lexeme + "\"";
            error_stack.push_back(error);
            return make_pair(false,ERROR);
          }
          else
          {
              symbol_table_entry symtab_entry = this->symtab->get_entry(fun_exp->function_name);//will return the arg_count
              vector<pair<token,token_type>> parameters = symtab_entry.parameters;
              token_type function_return_type = symtab_entry.return_type;
              bool final_result = true;
              if(parameters.size() != fun_exp->arguments.size()){
                    string error = "ERROR at line " + to_string(fun_exp->function_name.line_number) + " : Function \"" + fun_exp->function_name.lexeme + "\" expects " + to_string(parameters.size()) + " arguments, " + to_string(fun_exp->arguments.size()) + " given";
                    error_stack.push_back(error);
                    final_result = false;
              }

              for(int i = 0; i<fun_exp->arguments.size(); i++){
                  auto exp_result = analyse_expression(fun_exp->arguments[i]);
                  if(exp_result.second != parameters[i].second){
                    string error = "ERROR at line " + to_string(fun_exp->function_name.line_number) + " : Function \"" + fun_exp->function_name.lexeme + "\" Type Mismatch: Parameter \"" + parameters[i].first.lexeme + "\" expects " + get_token_type(parameters[i].second)+ ", " + get_token_type(exp_result.second)+ " given ";
                    error_stack.push_back(error);
                    final_result = false;
                  }
                  final_result = final_result && exp_result.first;
              }
              return make_pair(final_result,final_result?function_return_type:ERROR);
          }
}
