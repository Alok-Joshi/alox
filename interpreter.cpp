#include "interpreter.h"
#include "environment.h"
#include "ast.h"
#include <iostream>


using namespace std;
using namespace ast;




interpreter:: interpreter(vector<statement*> &statements){

            this->statements = statements;
}

void interpreter:: interpret(){
            
            
            auto env = environment::get_environment();
            env->push_scope();
            for(auto &statement: statements){

                statement->execute();
            }
            env->pop_scope();
}






