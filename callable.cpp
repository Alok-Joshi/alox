#include "callable.h"
#include "environment.h"


using namespace tok;
using namespace std;
using namespace ast;

callable:: callable(vector<token> &parameters, statement* body): parameters(parameters),body(body) {}

any callable:: call(vector<any> arguments){

    // Algorithm:
    // 1) Create a new scope. (push_scope)
    // 2) Now assign parameters to values and put them in the environment
    // 3) now execute block
    // 4) once block execution done, pop scope
    
        
        auto env = environment:: get_environment();
        
        env->push_scope();

        for(int i = 0; i<parameters.size(); i++){

            env->add_variable(parameters[i], arguments[i]);

        }

        body->execute();
        env->pop_scope();
        return NULL;

}

