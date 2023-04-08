#ifndef AST_H
#define AST_H

#include <string>
#include "token.h"

namespace ast {

    typedef enum{
        
        PRINT_STATEMENT,
        EXPRESSION_STATEMENT,
        DECLARATION_STATEMENT,

    } stmt_type;

    class expression{
        public:
        virtual void print_expression() = 0; //pure virtual function
        virtual double evaluate() = 0;
    };

    class statement {

        public:
        virtual void execute() = 0;

    };

    class expression_statement: public statement{

          expression *exp;
          public:
          expression_statement(expression *exp);
          void execute();

    };
    class declaration_statement: public statement{

          expression *exp;
          public:
          declaration_statement(expression *exp);
          void execute();

    };
    class print_statement: public statement{
        
          expression *exp;
          public:
          print_statement(expression *exp);
          void execute();

    };

    class binary_expression : public expression{
        expression * left;
        tok::token optr;
        expression *right;
        public: 
        binary_expression(expression *left, tok::token &optr, expression *right);
        void print_expression();
        double evaluate();
        
    };

    class unary_expression : public expression{
        tok::token optr;
        expression * right;

        public: 
        unary_expression(tok::token &optr, expression *right);
        void print_expression();
        double evaluate();
    };

    class literal_expression : public expression{
        tok::token literal;
        
        public: 
        literal_expression(tok:: token &literal);
        void print_expression();
        double evaluate();
    };

    class group_expression : public expression{
        expression*  exp;
        
        public: 
        group_expression(expression* exp);
        void print_expression();
        double evaluate();
    };

}


#endif
