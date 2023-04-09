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
        return this->parse_equality();

}


expression* parser:: parse_equality(){

    expression *left = parse_comparison();
    unordered_set<token_type> valid_operators = {BANG_EQUAL,EQUAL};

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

        return parse_literal(); 

}


expression* parser:: parse_literal(){
    
    unordered_set<token_type> valid_types = {IDENTIFIER,NUMBER,STRING,TRUE,FALSE,NIL};

    if(match(valid_types)){

        token literal_obj = get_literal();
        expression* litexpr = new literal_expression(literal_obj);
        return litexpr;
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
        auto identifier = consume_token(IDENTIFIER);
        consume_token(EQUAL);

        auto exp = parse_expression();
        statement * declaration_stmt = new declaration_statement(exp,identifier);

        consume_token(SEMICOLON);
        return declaration_stmt;


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
    else{
        
        //its an expression statement. this will change as we add more statement types

        return parse_expression_statement();

        

    }

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


