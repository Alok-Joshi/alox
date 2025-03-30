#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include "environment.h"
#include <any>



namespace ast {

   class visitor;

   class ast_node {

        public:
        virtual std::any accept(visitor *v) = 0;
        virtual ~ast_node() {}

    };

    class expression : public ast_node {

        public:
        int line_number;
        tok::token_type expression_type; //STRING, NUMBER
        virtual void print_expression() = 0; //pure virtual function
        expression(int line_number);
    };

    class statement : public ast_node {

        public:
        int line_number;
        statement(int line_number);

    };
    class conditional_statement: public statement{

          public:
          std::unique_ptr<expression> expr;
          std::unique_ptr<statement> if_statements;
          std::unique_ptr<statement> else_statements;
          conditional_statement(std::unique_ptr<expression> expr, std::unique_ptr<statement> if_statements, std::unique_ptr<statement> else_statements,int line_number);
          std::any accept(visitor *v);


    };
    class block_statement: public statement{

          public:
          std::vector<std::unique_ptr<statement>> statements;
          block_statement(std:: vector<std::unique_ptr<statement>> statements,int line_number);
          std::any accept(visitor *v);

    };
    class while_statement: public statement{

        public:
        std::unique_ptr<expression> expr;
        std::unique_ptr<statement> statements;
        while_statement(std::unique_ptr<expression> expr, std::unique_ptr<statement> statements,int line_number);
        std::any accept(visitor *v);


    };
    class for_statement: public statement{

        public:
        std::unique_ptr<statement> part1; //the declaration or assignment statement
        std::unique_ptr<expression> part2; //the expression which is checked for truth value;
        std::unique_ptr<expression> part3; //the operation done after every iteration of the loop
        std::unique_ptr<statement> statements;
        for_statement(std::unique_ptr<statement> part1, std::unique_ptr<expression> part2, std::unique_ptr<expression> part3, std::unique_ptr<statement> statements,int line_number);
        std::any accept(visitor *v);


    };


    class expression_statement: public statement{

          public:
            std::unique_ptr<expression> exp;
          expression_statement(std::unique_ptr<expression> exp,int line_number);
          std::any accept(visitor *v);


    };
    class declaration_statement: public statement{

          public:
          std::unique_ptr<expression> exp;
          tok:: token variable_name;
          tok:: token_type variable_type;
          declaration_statement(std::unique_ptr<expression> exp,tok::token variable,tok::token_type variable_type,int line_number);
          std::any accept(visitor *v);


    };
    class print_statement: public statement{

          public:
          std::unique_ptr<expression> exp;
          print_statement(std::unique_ptr<expression> exp, int line_number);
          std::any accept(visitor *v);



    };
    class input_statement: public statement {

         public:
         tok::token input_reciever_variable;
         tok::token_type input_type;
         input_statement(tok::token input_reciever_variable,tok::token_type input_type,int line_number);
         std::any accept(visitor *v);


    };

    class return_statement: public statement{

          public:
          std::unique_ptr<expression> return_exp;
          return_statement(std::unique_ptr<expression> return_exp,int line_number);
          std::any accept(visitor *v);



    };

    class function_declaration_statement: public statement{

          public:
          tok:: token function_name;
          tok:: token_type return_type;
          std:: vector<std:: pair<tok::token,tok::token_type>> parameters;
          std::unique_ptr<statement> block;
          function_declaration_statement(tok::token function_name, std:: vector<std:: pair<tok::token,tok::token_type>> &parameters, std::unique_ptr<statement> block,tok::token_type return_type,int line_number);
          std::any accept(visitor *v);


    };



    class binary_expression : public expression{
        public:
        std::unique_ptr<expression> left;
        tok::token optr;
        std::unique_ptr<expression> right;
        binary_expression(std::unique_ptr<expression> left, tok::token &optr, std::unique_ptr<expression> right,int line_number);
        void print_expression();
        std::any accept(visitor *v);


    };

    class logical_expression : public binary_expression{
        public:
        logical_expression(std::unique_ptr<expression> left, tok::token &optr, std::unique_ptr<expression> right,int line_number);
        void print_expression();
        std::any accept(visitor *v);


    };

;

    class unary_expression : public expression{
        public:
        tok::token optr;
        std::unique_ptr<expression> right;
        unary_expression(tok::token &optr, std::unique_ptr<expression> right,int line_number);
        void print_expression();
        std::any accept(visitor *v);


    };

    class literal_expression : public expression{

        public:
        tok::token literal;
        literal_expression(tok:: token &literal,int line_number);
        void print_expression();
        std::any accept(visitor *v);

    };

    class variable_literal_expression: public expression{

        public:
        tok::token variable_name;
        variable_literal_expression(tok:: token &variable_name,int line_number);
        tok:: token get_variable_name();
        void print_expression();
        std::any accept(visitor *v);

    };

    class function_call_expression: public expression {
        //the flow of evaluate:
        // 1) Evaluate function_identifier_expression
        // 2) Now access the environment and get the corresponding callable object
        // 3) call the call() function of the callable object which will return the any value

        public:
        tok::token function_name;
        std::vector<std::unique_ptr<expression>> arguments;
        function_call_expression(tok::token function_name, std::vector<std::unique_ptr<expression>> arguments,int line_number);
        void print_expression();
        std::any accept(visitor *v);


    };

    class group_expression : public expression{
        public:
        std::unique_ptr<expression>  exp;
        group_expression(std::unique_ptr<expression> exp,int line_number);
        void print_expression();
    };

    class class_declaration_statement: public statement {
        public:
        tok:: token variable_name;
        std::vector<std::unique_ptr<statement>> methods;
        class_declaration_statement(tok:: token variable_name, std:: vector<std::unique_ptr<statement>> methods,int line_number);
        std:: any accept(visitor* vt);
        std:: any execute();

    };

    class visitor {

    public:
    virtual void visit_conditional_statement(conditional_statement* stmt) = 0;
    virtual void visit_block_statement(block_statement* stmt) = 0;
    virtual void visit_while_statement(while_statement* stmt) = 0;
    virtual void visit_for_statement(for_statement* stmt) = 0;
    virtual void visit_expression_statement(expression_statement* stmt) = 0;
    virtual void visit_declaration_statement(declaration_statement* stmt) = 0;
    virtual void visit_print_statement(print_statement* stmt) = 0;
    virtual void visit_input_statement(input_statement* stmt) = 0;
    virtual void visit_return_statement(return_statement* stmt) = 0;
    virtual void visit_function_declaration_statement(function_declaration_statement* stmt) = 0;

    virtual std::any visit_binary_expression(binary_expression* exp) = 0;
    virtual std::any visit_logical_expression(logical_expression *exp) = 0;
    virtual std::any visit_unary_expression(unary_expression * exp) = 0;
    virtual std::any visit_literal_expression(literal_expression *exp) = 0;
    virtual std::any visit_variable_literal_expression(variable_literal_expression* exp) = 0;
    virtual std::any visit_function_call_expression(function_call_expression * exp) = 0;

    };
    class semantic_analyser: public visitor {


        std::vector<std::unique_ptr<ast::statement>> ast;
        std::unique_ptr<symbol_table> symtab;
        void block_resolver(ast::block_statement* block);
        bool return_encountered;
        std::string get_type_mismatch_error(expression *expr);
        int get_line_number(expression* expr);

        public:
        std:: vector<std::string> error_stack;
        semantic_analyser(std::vector<std::unique_ptr<ast::statement>> ast);
        void analyse_program();
        void visit_conditional_statement(conditional_statement* stmt);
        void visit_block_statement(block_statement* stmt);
        void visit_while_statement(while_statement* stmt);
        void visit_for_statement(for_statement* stmt);
        void visit_expression_statement(expression_statement* stmt);
        void visit_declaration_statement(declaration_statement* stmt);
        void visit_print_statement(print_statement* stmt);
        void visit_input_statement(input_statement* stmt);
        void visit_return_statement(return_statement* stmt);
        void visit_function_declaration_statement(function_declaration_statement* stmt);

        std::any visit_binary_expression(binary_expression* exp) ;
        std::any visit_logical_expression(logical_expression *exp) ;
        std::any visit_unary_expression(unary_expression * exp) ;
        std::any visit_literal_expression(literal_expression *exp) ;
        std::any visit_variable_literal_expression(variable_literal_expression* exp) ;
        std::any visit_function_call_expression(function_call_expression * exp) ;




    };



}


#endif
