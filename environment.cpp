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

    map<string,any> newscope;
    this->scopes.push_back(newscope);

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

