#include "parser.h"
#include "environment.h"
#include "token.h"
#include "ast.h"
#include <iostream>
#include <unordered_set>

using namespace std;
using namespace tok;
using namespace ast;





parser:: parser(vector<token> &tokens){
        this->current = 0;
        this->tokens = tokens;
}




expression* parser:: parse_expression(){
        return this->parse_assignment();

}
expression * parser :: parse_assignment(){

        
            expression * left = parse_equality();

            if(typeid((*left)) == typeid(variable_literal_expression)){

                
                if(match(EQUAL)){

                     auto tok = consume_token(EQUAL); 
                     expression * expr = parse_assignment();
                     left = new binary_expression(left,tok,expr);
                     return left;

                }

            }

            return left;


}


expression* parser:: parse_equality(){


    expression *left = parse_comparison();
    unordered_set<token_type> valid_operators = {BANG_EQUAL,EQUAL_EQUAL};

    while(match(valid_operators)){
        
        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        expression *right = parse_comparison();
        left = new binary_expression(left,optr,right);
            
    }

    return left;

}


expression* parser:: parse_comparison(){
        expression *left = parse_addsub();
        unordered_set<token_type> valid_operators = {GREATER,GREATER_EQUAL,LESS,LESS_EQUAL,EQUAL_EQUAL};


        while(match(valid_operators)){
        
        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        expression *right = parse_addsub();
        left = new binary_expression(left,optr,right);
            
        }

    return left;

}

expression* parser:: parse_addsub(){

        expression *left = parse_multdiv();
        unordered_set<token_type> valid_operators = {PLUS,MINUS};


        while(match(valid_operators)){
        
        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        expression *right = parse_multdiv();
        left = new binary_expression(left,optr,right);
            
        }

    return left;
 
}

expression* parser:: parse_multdiv(){

        expression *left = parse_unary();
        unordered_set<token_type> valid_operators = {STAR,SLASH};

        while(match(valid_operators)){
        
        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        expression *right = parse_unary();
        left = new binary_expression(left,optr,right);
            
        }

    return left;


}

expression* parser:: parse_unary(){
        
        unordered_set<token_type> valid_types = {BANG,MINUS};
        while(match(valid_types)){
            token optr = get_operator();
            expression *right = parse_unary();
            return new unary_expression(optr,right);
        }

        return parse_call(); 

}

expression *parser:: parse_call(){


         expression *identifier_name = parse_literal();
         vector<expression*> arguments;

         if(typeid((*identifier_name)) == typeid(variable_literal_expression)){


             if(match(LEFT_PAREN)){
             consume_token(LEFT_PAREN); //will be later replaced by outer while loop for multiple calls

             if(!match(RIGHT_PAREN)){ //implies there is an argument given

                 arguments.push_back(parse_expression());
                 while(!match(RIGHT_PAREN)){

                     consume_token(COMMA);
                     arguments.push_back(parse_expression());


                 }

            }

             consume_token(RIGHT_PAREN);

            }
         }

         if(arguments.size()>0){
             expression * call_expr = new function_call_expression(identifier_name,arguments);
             return call_expr;

         }
         else
         {

                return identifier_name;
                
         }

}
expression* parser:: parse_literal(){
    
    unordered_set<token_type> valid_types = {IDENTIFIER,NUMBER,STRING,TRUE,FALSE,NIL};

    if(match(valid_types)){

        token literal_obj = get_literal();
        if(literal_obj.type == IDENTIFIER){
            expression *litvarexp = new variable_literal_expression(literal_obj);
            return litvarexp;

        }
        else
        {
            expression* litexpr = new literal_expression(literal_obj);
            return litexpr;

        }
    }
    else
    {
        if(tokens[current].type == LEFT_PAREN)
        {
                current++; //consume the opening (
                expression *expr =  parse_expression(); //expression inside (expression)

                if(tokens[current].type != RIGHT_PAREN)
                {
                    //error, expected ). TODO: need to dry run how this error will look like
                }

                return expr;
        }
        return NULL;
    }
}

bool parser:: match(unordered_set<token_type> &valid_types){

        return valid_types.count(tokens[current].type);

}
bool parser :: match(token_type tt){

        return tokens[current].type == tt;
}

token parser:: get_operator(){

    token optr = tokens[current];
    ++current;
    return optr;
}

token parser:: get_literal(){
    token literal = tokens[current];
    ++current;
    return literal;
}


token parser:: consume_token(unordered_set<token_type> &valid_types){

    token literal = tokens[current];
    if(valid_types.count(literal.type)){

        return literal;
    }
    else
    {
        throw  "expected token at "; //TODO: need to work on error handling
    }

}
token parser:: peak(){
    //simply returns the current token, does not consume it (current remains where it is)
    return tokens[current];

}
token parser:: consume_token(token_type tt){

    token tk = tokens[current];
    if(tt == tk.type){

        current++;
        return tk;
    }
    else
    {
        throw "expected token at"; //TODO: need to work on error handling;
    }
}
// PARSING STATEMENTS

vector<statement*> parser:: parse_program(){

    vector<statement*> declarations;

    while(!match(END_OF_FILE)){

        declarations.push_back(parse_declaration());

    }

    consume_token(END_OF_FILE);

    return declarations;

}

statement* parser:: parse_declaration(){

        
    if(match(VAR)){ //declarations start with VAR

        consume_token(VAR);

        auto variable_name = peak();
        auto exp = parse_expression();
        statement * declaration_stmt = new declaration_statement(exp,variable_name);

        consume_token(SEMICOLON);
        return declaration_stmt;


    }
    else if(match(FUN)){

        return parse_function_declaration_statement();

    }
    else //its a different kind of statement, so direct to generic statement
    {
   
        return parse_statement();
     
    }
}


statement* parser:: parse_statement(){

    if(match(PRINT)){


        return parse_print_statement();

    }
    else if(match(LEFT_BRACE)){

        return parse_block_statement();

    }
    else if(match(IF)){


        return parse_conditional_statement();

    }
    else if(match(WHILE)){


        return parse_while_statement();
    }
    else if(match(FOR)){


        return parse_for_statement();
    }
    
    else{
        
        //its an expression statement. this will change as we add more statement types
        return parse_expression_statement();


    }

}

statement* parser:: parse_for_statement(){


        consume_token(FOR);
        consume_token(LEFT_PAREN);
        statement *part1 = NULL;
        expression *part2 = NULL;
        expression *part3 = NULL;
        statement *statements = NULL;

        if(peak().type != SEMICOLON){
            //TODO: make an empty statement class for empty semicolons
            if(match(VAR)) part1 = parse_declaration();
            else part1 = parse_expression_statement();
        }
        
        if(peak().type != SEMICOLON){

            //implies there is a condition
    
            part2 = parse_expression();
        }
        consume_token(SEMICOLON);

        if(peak().type != RIGHT_PAREN){

            
            part3 = parse_expression();
                
        }


        consume_token(RIGHT_PAREN);
        statements = parse_statement();

        statement * for_stmt = new for_statement(part1,part2,part3,statements);
        return for_stmt;

}
statement* parser:: parse_while_statement(){

        consume_token(WHILE);
        consume_token(LEFT_PAREN);
        expression *expr = parse_expression();
        consume_token(RIGHT_PAREN);

        statement * statements = parse_statement();

        statement *whilestmt = new while_statement(expr,statements);
        return whilestmt;

}

statement* parser::  parse_conditional_statement(){

        consume_token(IF);
        consume_token(LEFT_PAREN);
        expression *expr = parse_expression();
        consume_token(RIGHT_PAREN);

        statement* ifstatements = parse_statement();
        statement* elsestatement = NULL;

        if(match(ELSE)){

            consume_token(ELSE);
            elsestatement = parse_statement();

        }

        statement* ifstmt = new conditional_statement(expr,ifstatements,elsestatement);
        return ifstmt;
}

statement * parser:: parse_block_statement(){

    consume_token(LEFT_BRACE);
    vector<statement*> statements;

    while(!match(RIGHT_BRACE)){

        statements.push_back(parse_statement());
    }

    consume_token(RIGHT_BRACE);

    statement* block = new block_statement(statements);
    return block;

}


statement* parser:: parse_print_statement(){
        
    
    consume_token(PRINT);
    expression* exp = parse_expression();
    consume_token(SEMICOLON);

    statement* ps = new print_statement(exp);
    return ps;

}


statement* parser:: parse_expression_statement(){

    expression* exp = parse_expression();
    consume_token(SEMICOLON);

    statement *ex = new expression_statement(exp);
    return ex;
}

statement* parser:: parse_function_declaration_statement(){

        consume_token(FUN);
        auto function_name = consume_token(IDENTIFIER);
        vector<token> parameters;
        consume_token(LEFT_PAREN);
        
        if(match(IDENTIFIER)){

        parameters.push_back(consume_token(IDENTIFIER));
        while(!match(RIGHT_PAREN)){

            consume_token(COMMA);
            parameters.push_back(consume_token(IDENTIFIER));


        }

        }
        consume_token(RIGHT_PAREN);
        statement* statements = parse_block_statement();

        statement* fndec_stmt = new function_declaration_statement(function_name,parameters,statements);

        return fndec_stmt;

}


