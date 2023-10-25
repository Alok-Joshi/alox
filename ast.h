#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include "environment.h"
#include <any>



namespace ast {

   class visitor;

   class ast_node {

        public:
        virtual void accept(visitor *v) = 0;

    };

    class expression : public ast_node {

        protected:
        int line_number;
        public:
        tok::token_type expression_type; //STRING, NUMBER
        
        virtual void print_expression() = 0; //pure virtual function
        expression(int line_number);
    };

    class statement : public ast_node {

        protected:
        int line_number;
        public:
        statement(int line_number);

    };
    class conditional_statement: public statement{
            
          expression *expr;
          statement* if_statements;
          statement *else_statements;
          public:
          conditional_statement(expression *expr, statement*  if_statements, statement *else_statements,int line_number);
          void accept(visitor *v);
          

    };
    class block_statement: public statement{
            
          std::vector<statement*> statements;
          public:
          block_statement(std:: vector<statement*> &statements,int line_number);
          void accept(visitor *v);
          
    };
    class while_statement: public statement{

        expression *expr;
        statement *statements;
        public:
        while_statement(expression *expr, statement *statements,int line_number);
        void accept(visitor *v);
        

    };
    class for_statement: public statement{

        statement * part1; //the declaration or assignment statement
        expression * part2; //the expression which is checked for truth value;
        expression * part3; //the operation done after every iteration of the loop
        statement * statements;
        public:
        for_statement(statement *part1, expression *part2, expression* part3,statement* statements,int line_number);
        void accept(visitor *v);
        

    };


    class expression_statement: public statement{

          expression *exp;
          public:
          expression_statement(expression *exp,int line_number);
          void accept(visitor *v);
          

    };
    class declaration_statement: public statement{

          public:
          expression *exp;
          tok:: token variable_name;
          tok:: token_type variable_type;
          declaration_statement(expression *exp,tok::token variable,tok::token_type variable_type,int line_number);
          void accept(visitor *v);
          

    };
    class print_statement: public statement{
        
          expression *exp;
          public:
          print_statement(expression *exp,int line_number);
          void accept(visitor *v);
          
          

    };
    class input_statement: public statement {

         tok::token input_reciever_variable;
         tok::token_type input_type;
         public:
         input_statement(tok::token input_reciever_variable,tok::token_type input_type,int line_number);
         void accept(visitor *v);
         

    };

    class return_statement: public statement{

          expression *return_exp;
          public:
          return_statement(expression *return_exp,int line_number);
          void accept(visitor *v);
          


    };

    class function_declaration_statement: public statement{
        
          tok:: token function_name;
          tok:: token_type return_type;
          std:: vector<std:: pair<tok::token,tok::token_type>> parameters;
          statement* block;
          public:
          function_declaration_statement(tok::token function_name, std:: vector<std:: pair<tok::token,tok::token_type>> &parameters, statement* block,tok::token_type return_type,int line_number);
          void accept(visitor *v);
          

    };



    class binary_expression : public expression{
        protected:
        expression * left;
        tok::token optr;
        expression *right;
        public: 
        binary_expression(expression *left, tok::token &optr, expression *right,int line_number);
        void print_expression();
        void accept(visitor *v);
        
        
    };

    class logical_expression : public binary_expression{
        public: 
        logical_expression(expression *left, tok::token &optr, expression *right,int line_number);
        void print_expression();
        void accept(visitor *v);
        
        
    };

;

    class unary_expression : public expression{
        tok::token optr;
        expression * right;
        public: 
        unary_expression(tok::token &optr, expression *right,int line_number);
        void print_expression();
        void accept(visitor *v);
        
        
    };

    class literal_expression : public expression{
        
        tok::token literal;
        public: 
        literal_expression(tok:: token &literal,int line_number);
        void print_expression();
        void accept(visitor *v);
        
    };

    class variable_literal_expression: public expression{
        
        tok::token variable_name;
        public: 
        variable_literal_expression(tok:: token &variable_name,int line_number);
        tok:: token get_variable_name();
        void print_expression();
        void accept(visitor *v);
        
    };

    class function_call_expression: public expression {
        //the flow of evaluate:
        // 1) Evaluate function_identifier_expression
        // 2) Now access the environment and get the corresponding callable object 
        // 3) call the call() function of the callable object which will return the any value

        tok::token function_name;
        std::vector<expression*> arguments;
        public:
        function_call_expression(tok::token function_name, std::vector<expression*> &arguments,int line_number);
        void print_expression();
        void accept(visitor *v);
        

    };

    class group_expression : public expression{
        expression*  exp;
        
        public: 
        group_expression(expression* exp,int line_number);
        void print_expression();
    };

    class class_declaration_statement: public statement {
        tok:: token variable_name;
        std::vector<statement*> methods;
        public:
        class_declaration_statement(tok:: token variable_name, std:: vector<statement*> methods,int line_number);
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

    virtual void visit_binary_expression(binary_expression* exp) = 0; 
    virtual void visit_logical_expression(logical_expression *exp) = 0;
    virtual void visit_unary_expression(unary_expression * exp) = 0;
    virtual void visit_literal_expression(literal_expression *exp) = 0;
    virtual void visit_variable_literal_expression(variable_literal_expression* exp) = 0;
    virtual void visit_function_call_expression(function_call_expression * exp) = 0;

    };
    class semantic_analyser: public visitor {

                
        std::vector<ast:: statement*> ast;
        symbol_table* symtab;
        void block_resolver(ast::block_statement* block);
        bool return_encountered;
        std::string get_type_mismatch_error(expression *expr);
        int get_line_number(expression* expr);

        public:
        std:: vector<std::string> error_stack;
        semantic_analyser(std:: vector<ast:: statement*> ast);
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

     void visit_binary_expression(binary_expression* exp) ; 
     void visit_logical_expression(logical_expression *exp) ;
     void visit_unary_expression(unary_expression * exp) ;
     void visit_literal_expression(literal_expression *exp) ;
     void visit_variable_literal_expression(variable_literal_expression* exp) ;
     void visit_function_call_expression(function_call_expression * exp) ;


         

    };
        


}


#endif
