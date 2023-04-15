#ifndef CALLABLE_H
#define CALLABLE_H

#include<vector>
#include<any>
#include "ast.h"
#include "token.h"
#include "environment.h"



class callable{


    std:: vector<tok::token> parameters;
    ast::statement* body;

    public:
    callable(std::vector<tok::token> &parameters, ast::statement* body);
    std:: any call(std::vector<std::any> arguments);

};


#endif


