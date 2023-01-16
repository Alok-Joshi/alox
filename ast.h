#ifndef AST_H
#define AST_H

#include <string>
#include "token.h"

namespace ast {

    class expression{
        //TODO: Implement print expression function for all the below classes

        virtual void print_expression();
    };

    class binary_expression : public expression{
        expression * left;
        tok::token optr;
        expression *right;
        
        binary_expression(expression *left, tok::token &optr, expression *right);
    };

    class unary_expression : public expression{
        expression * left;
        tok::token optr;
        
        unary_expression(expression *left, tok::token &optr);
    };

    class literal_expression : public expression{
        std::string literal;
        
        literal_expression(std:: string &literal);
    };

    class group_expression : public expression{
        expression  exp;
        
        group_expression(expression* exp);
    };

}


#endif
