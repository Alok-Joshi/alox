#include "callable.h"


using namespace tok;
using namespace std;
using namespace ast;

callable:: callable(vector<token> &parameters, statement* body): parameters(parameters),body(body) {}

any callable:: call(vector<any> arguments){



}

