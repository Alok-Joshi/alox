#ifndef AST_H
#define AST_H

#include <string>
#include "token.h"

namespace ast {

    class expression{
        //TODO: Implement print expression function for all the below classes
        public:
        virtual void print_expression() = 0; //pure virtual function
    };

    class binary_expression : public expression{
        expression * left;
        tok::token optr;
        expression *right;
        public: 
        binary_expression(expression *left, tok::token &optr, expression *right);
        void print_expression();
    };

    class unary_expression : public expression{
        tok::token optr;
        expression * right;

        public: 
        unary_expression(tok::token &optr, expression *right);
        void print_expression();
    };

    class literal_expression : public expression{
        tok::token literal;
        
        public: 
        literal_expression(tok:: token &literal);
        void print_expression();
    };

    class group_expression : public expression{
        expression*  exp;
        
        public: 
        group_expression(expression* exp);
        void print_expression();
    };

}


#endif
