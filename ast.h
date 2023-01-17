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
        void print_expression();
    };

    class unary_expression : public expression{
        tok::token optr;
        expression * right;
        
        unary_expression(tok::token &optr, expression *right);
        void print_expression();
    };

    class literal_expression : public expression{
        tok::token literal;
        
        literal_expression(token:: token &literal);
        void print_expression();
    };

    class group_expression : public expression{
        expression  exp;
        
        group_expression(expression* exp);
        void print_expression();
    };

}


#endif
