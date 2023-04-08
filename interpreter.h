#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include <vector>

class interpreter {

    public:
    interpreter(std::vector<ast::statement*>&);
    std::vector<ast::statement*> statements;
    void interpret();

};


#endif
