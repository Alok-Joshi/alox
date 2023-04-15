#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
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
    class conditional_statement: public statement{
            
          expression *expr;
          statement* if_statements;
          statement *else_statements;
          public:
          void execute();
          conditional_statement(expression *expr, statement*  if_statements, statement *else_statements);
    };
    class block_statement: public statement{
            
          std::vector<statement*> statements;
          public:
          void execute();
          block_statement(std:: vector<statement*> &statements);
    };
    class while_statement: public statement{

        expression *expr;
        statement *statements;
        public:
        void execute();
        while_statement(expression *expr, statement *statements);

    };
    class for_statement: public statement{

        statement * part1; //the declaration or assignment statement
        expression * part2; //the expression which is checked for truth value;
        expression * part3; //the operation done after every iteration of the loop
        statement * statements;
        public:
        void execute();
        for_statement(statement *part1, expression *part2, expression* part3,statement* statements);

    };


    class expression_statement: public statement{

          expression *exp;
          public:
          expression_statement(expression *exp);
          void execute();

    };
    class declaration_statement: public statement{

          expression *exp;
          tok:: token variable_name;
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

    class function_declaration_statement: public statement{
        
          tok:: token function_name;
          std:: vector<tok:: token> parameters;
          statement* block;
          public:
          function_declaration_statement(tok::token function_name, std:: vector<tok::token> &parameters, statement* block);
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

    class variable_literal_expression: public expression{
        tok::token variable_name;
        
        public: 
        variable_literal_expression(tok:: token &variable_name);
        void print_expression();
        std::any evaluate();
    };

    class function_call_expression: public expression {
        //the flow of evaluate:
        // 1) Evaluate function_identifier_expression
        // 2) Now access the environment and get the corresponding callable object 
        // 3) call the call() function of the callable object which will return the any value

        expression *function_name;
        std::vector<expression*> arguments;
        public:
        function_call_expression(expression* function_name, std::vector<expression*> &arguments);
        void print_expression();
        std:: any evaluate();

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
