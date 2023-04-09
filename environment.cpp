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



void environment:: add_variable(token identifier, any value){

        if(env_map.count(identifier.lexeme)){
            //implies identifier declared before

            throw "redeclaration error";
        }
        else
        {
            env_map[identifier.lexeme] = value;
        }

}

any environment:: get_variable(token identifier){
        
        if(!env_map.count(identifier.lexeme)){

            throw "undeclared variable " + identifier.lexeme;
        }
        
        return env_map[identifier.lexeme];

}

