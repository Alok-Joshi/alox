#include "interpreter.h"
#include "ast.h"
#include <iostream>


using namespace std;
using namespace ast;




interpreter:: interpreter(vector<statement*> &statements){

            this->statements = statements;
}

void interpreter:: interpret(){
            
            for(auto &statement: statements){

                statement->execute();
            }
}






