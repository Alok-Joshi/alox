#ifndef CALLABLE_H
#define CALLABLE_H

#include<vector>
#include<any>
#include "ast.h"
#include "token.h"



class callable{


    std:: vector<std::any> parameters;
    std:: any call(std::vector<std::any> arguments);
    ast::statement* body;

    public:
    callable(std::vector<tok::token> &parameters, ast::statement* body);

};


#endif


