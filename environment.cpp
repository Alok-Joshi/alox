#include "environment.h"

using namespace std;
using namespace tok;



environment* environment:: env_variable = NULL;
environment* environment:: get_environment(){
    
        if(env_variable == NULL){
            
            env_variable = new environment();
        
        }
        return env_variable;

}


void environment:: push_scope(){

    map<string,any> new_scope;
    this->scopes.push_back(new_scope);

}

void environment:: pop_scope(){

    this->scopes.pop_back();


}
void environment:: add_variable(token identifier, any value){
    //API specifically designed for declaration statements

        int top_index = scopes.size()-1;
        if(scopes[top_index].count(identifier.lexeme)){
            //implies identifier declared before

            throw "redeclaration error";
        }
        else
        {
            scopes[top_index][identifier.lexeme] = value;
        }
}

void environment:: set_variable(token identifier, any value){

    //API for eexpressions involving assignment operators

    //we are traversing from the innermost (latest) scope to the outermost scope. If variable is not found, error is thrown.

    for(int i = scopes.size()-1; i>=0; i--){


        if(scopes[i].count(identifier.lexeme)){


            scopes[i][identifier.lexeme] = value;
            return;

        }
    

    }
    throw "Unknown variable";
        

}

any environment:: get_variable(token identifier){
        
    //searches all scopes, starting from the innermost (latest) scope to the outermost scope. If variable no found, error is thrown.
    for(int i = scopes.size()-1; i>=0; i--){


        if(scopes[i].count(identifier.lexeme)){


            return scopes[i][identifier.lexeme];

        }
    

    }

    throw "undeclared variable " + identifier.lexeme;

}


//SYMBOL TABLE IMPLEMENTATIONS


bool symbol_table:: resolve_identifier(tok:: token identifier){

    
        int stack_top = this->scopes.size()-1;

        while(stack_top>=0){

        
            if(this->scopes[stack_top].count(identifier.lexeme)) return true;
            else stack_top--;

        }

        return false;
}

bool symbol_table:: is_redeclaration(tok:: token identifier){

            return this->scopes[scopes.size()-1].count(identifier.lexeme);

}

void symbol_table:: start_scope(token name){


    this->function_tracker.push_back(name);
    this->start_scope();

}

void symbol_table:: start_scope(){

    map<string,any> new_scope;
    this->scopes.push_back(new_scope);

}

void symbol_table:: end_scope(bool is_function_scope){

    this->scopes.pop_back();

    if(is_function_scope){
        
        this->function_tracker.pop_back();

    }

}
void symbol_table:: modify_entry(token symbol, any symbol_information){
    //API specifically designed for declaration statements
            scopes[scopes.size()-1][symbol.lexeme] = symbol_information;

}



void symbol_table:: add_entry(token symbol, any symbol_information){
    //API specifically designed for declaration statements

        int top_index = scopes.size()-1;
        if(scopes[top_index].count(symbol.lexeme)){
            //implies identifier declared before

            throw "redeclaration error";
        }
        else
        {
            scopes[top_index][symbol.lexeme] = symbol_information;
        }
}


token symbol_table:: get_current_function(){


        if(this->function_tracker.size() == 0){

            throw "No Current Function"; //TODO: Improve this during error handling


        }

        else
        {
            
            int top_index = this->function_tracker.size() - 1;
            return this->function_tracker[top_index];
        }

}


