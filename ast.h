#ifndef AST_H
#define AST_H

#include <string>
#include "token.h"
#include "environment.h"
#include <any>

namespace ast {

    typedef enum{
        
        PRINT_STATEMENT,
        EXPRESSION_STATEMENT,
        DECLARATION_STATEMENT,

    } stmt_type;

    class expression{
        public:
        virtual void print_expression() = 0; //pure virtual function
        virtual std::any evaluate() = 0;
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
          tok:: token variable;
          public:
          declaration_statement(expression *exp,tok::token variable);
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
        std::any evaluate();
        
    };

    class unary_expression : public expression{
        tok::token optr;
        expression * right;

        public: 
        unary_expression(tok::token &optr, expression *right);
        void print_expression();
        std::any evaluate();
        
    };

    class literal_expression : public expression{
        tok::token literal;
        
        public: 
        literal_expression(tok:: token &literal);
        void print_expression();
        std::any evaluate();
    };

    class group_expression : public expression{
        expression*  exp;
        
        public: 
        group_expression(expression* exp);
        void print_expression();
        std::any evaluate();
    };

}


#endif
