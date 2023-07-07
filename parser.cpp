#include "parser.h"
#include "environment.h"
#include "token.h"
#include "ast.h"
#include <iostream>
#include <unordered_set>

using namespace std;
using namespace tok;
using namespace ast;




token_type get_type(token type_word){
        

    if(type_word.lexeme == "Void") return tok::VOID_TYPE;
    if(type_word.lexeme == "Number") return tok::NUMBER_TYPE;
    if(type_word.lexeme == "String") return tok::STRING_TYPE;

}

parser:: parser(vector<token> &tokens){
        this->current = 0;
        this->tokens = tokens;
}




expression* parser:: parse_expression(){
        return this->parse_assignment();

}
expression * parser :: parse_assignment(){

        
            expression * left = parse_logical_or();

            if(typeid((*left)) == typeid(variable_literal_expression)){

                
                if(match(EQUAL)){

                     auto tok = consume_token(EQUAL); 
                     expression * expr = parse_assignment();
                     left = new binary_expression(left,tok,expr,tok.line_number);
                     return left;

                }

            }

            return left;


}

expression *parser:: parse_logical_or(){

    expression *left = parse_logical_and();

    while(match(OR)){
        
        auto tok = consume_token(OR);
        expression *expr = parse_logical_and();
        left = new logical_expression(left,tok,expr,tok.line_number);

    }


    return left;



}

expression *parser:: parse_logical_and(){

    expression *left = parse_equality();

    while(match(AND)){
        
        auto tok = consume_token(AND);
        expression *expr = parse_equality();
        left = new logical_expression(left,tok,expr,tok.line_number);

    }

    return left;

}



expression* parser:: parse_equality(){


    expression *left = parse_comparison();
    unordered_set<token_type> valid_operators = {BANG_EQUAL,EQUAL_EQUAL};

    while(match(valid_operators)){
        
        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        expression *right = parse_comparison();
        left = new binary_expression(left,optr,right,optr.line_number);
            
    }

    return left;

}


expression* parser:: parse_comparison(){
        expression *left = parse_addsub();
        unordered_set<token_type> valid_operators = {GREATER,GREATER_EQUAL,LESS,LESS_EQUAL,EQUAL_EQUAL};


        while(match(valid_operators)){
        
        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        expression *right = parse_addsub();
        left = new binary_expression(left,optr,right,optr.line_number);
            
        }

    return left;

}

expression* parser:: parse_addsub(){

        expression *left = parse_multdiv();
        unordered_set<token_type> valid_operators = {PLUS,MINUS};


        while(match(valid_operators)){
        
        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        expression *right = parse_multdiv();
        left = new binary_expression(left,optr,right,optr.line_number);
            
        }

    return left;
 
}

expression* parser:: parse_multdiv(){

        expression *left = parse_unary();
        unordered_set<token_type> valid_operators = {STAR,SLASH};

        while(match(valid_operators)){
        
        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        expression *right = parse_unary();
        left = new binary_expression(left,optr,right,optr.line_number);
            
        }

    return left;


}

expression* parser:: parse_unary(){
        
        unordered_set<token_type> valid_types = {BANG,MINUS};
        while(match(valid_types)){
            token optr = get_operator();
            expression *right = parse_unary();
            return new unary_expression(optr,right,optr.line_number);
        }

        return parse_call(); 

}

expression *parser:: parse_call(){


         expression *exp = parse_literal();
         vector<expression*> arguments;

         int line_number = -1;

         if(typeid((*exp)) == typeid(variable_literal_expression)){


             if(match(LEFT_PAREN)){
             consume_token(LEFT_PAREN); //will be later replaced by outer while loop for multiple calls

             if(!match(RIGHT_PAREN)){ //implies there is an argument given

                 arguments.push_back(parse_expression());
                 while(!match(RIGHT_PAREN)){

                     consume_token(COMMA);
                     arguments.push_back(parse_expression());


                 }

            }

             line_number = consume_token(RIGHT_PAREN).line_number;

            }
         }

         if(line_number != -1){
             expression * call_expr = new function_call_expression(static_cast<variable_literal_expression*>(exp)->get_variable_name(),arguments,line_number);
             return call_expr;

         }
         else
         {

                return exp;
                
         }

}
expression* parser:: parse_literal(){
    
    unordered_set<token_type> valid_types = {IDENTIFIER,NUMBER_TYPE,STRING_TYPE,TRUE,FALSE,NIL};

    if(match(valid_types)){

        token literal_obj = get_literal();
        if(literal_obj.type == IDENTIFIER){
            expression *litvarexp = new variable_literal_expression(literal_obj,literal_obj.line_number);
            return litvarexp;

        }
        else
        {
            expression* litexpr = new literal_expression(literal_obj,literal_obj.line_number);
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

        current++;
        return literal;
    }
    else
    {
        string error = "ERROR at line " +to_string(literal.line_number) + " :  Expected token \"" + "\"";
        throw  error;
    }

}
token parser:: peak(){
    //simply returns the current token, does not consume it (current remains where it is)
    return tokens[current];

}
token parser:: consume_token(token_type valid_type){

    token tk = tokens[current];
    if(valid_type == tk.type){

        current++;
        return tk;
    }
    else
    {
        string token_error = (valid_type == tok::IDENTIFIER? "identifier" : enum_transalator.at(valid_type));

        string error = "ERROR at line: " +to_string(tk.line_number) + " Expected token \"" + token_error + "\"";
        throw  error;
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
statement* parser:: parse_return_statement(){


    int line_number = consume_token(RETURN).line_number;

    expression *expr = parse_expression();
    consume_token(SEMICOLON);
    
    auto rtsmt = new return_statement(expr,line_number);
    return rtsmt;


}

void parser:: synchronise(){


    while(!match(tok::END_OF_FILE)){
    
        if(tokens[current-1].type == SEMICOLON) return;

        unordered_set<token_type> statement_start = {FUN,WHILE,FOR,VAR,CLASS,INPUT_NUMBER,INPUT_STRING,PRINT,IF,RETURN,INPUT_NUMBER,INPUT_STRING};
        if(statement_start.count(tokens[current].type)) return;

        current++;

    }

}
statement* parser:: parse_declaration(){

        
    try {

        if(match(VAR)){ //declarations start with VAR

            int line_number = consume_token(VAR).line_number;
            consume_token(COLON);
            token_type variable_type = get_type(consume_token(TYPE));

            auto variable_name = peak();
            expression *exp = parse_expression();

            statement * declaration_stmt = new declaration_statement(exp,variable_name,variable_type,line_number);

            consume_token(SEMICOLON);

            return declaration_stmt;


        }
        else if(match(FUN)){

                return parse_function_declaration_statement();

        }
        else if(match(CLASS)){
        
            return parse_class_declaration_statement();

        }
        else //its a different kind of statement, so direct to generic statement
        {
       
            return parse_statement();
         
        }
    }
    catch(string error){
            
        cout<<error<<endl;
        this->synchronise();
        error_status = true;

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
    else if(match(RETURN)){

        return parse_return_statement();

    }

    else if(match(tok::INPUT_STRING)){

        
        return parse_string_input_statement();

    }

    else if(match(INPUT_NUMBER)){

        return parse_number_input_statement();

    }
    
    else{
        
        //its an expression statement. this will change as we add more statement types
        return parse_expression_statement();


    }

}

statement * parser:: parse_number_input_statement(){
    

    consume_token(tok::INPUT_NUMBER);
    token input_reciever = consume_token(tok::IDENTIFIER);
    consume_token(tok::SEMICOLON);

    auto number_input_stmt = new input_statement(input_reciever,tok::NUMBER_TYPE,input_reciever.line_number);
    return number_input_stmt;


}

statement * parser:: parse_string_input_statement(){
    

    consume_token(tok::INPUT_STRING);
    token input_reciever = consume_token(tok::IDENTIFIER);
    consume_token(tok::SEMICOLON);

    auto str_input_stmt = new input_statement(input_reciever,tok::STRING_TYPE,input_reciever.line_number);
    return str_input_stmt;


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


        int line_number = consume_token(RIGHT_PAREN).line_number;
        statements = parse_statement();

        statement * for_stmt = new for_statement(part1,part2,part3,statements,line_number);
        return for_stmt;

}
statement* parser:: parse_while_statement(){

        consume_token(WHILE);
        consume_token(LEFT_PAREN);
        expression *expr = parse_expression();
        int line_number = consume_token(RIGHT_PAREN).line_number;

        statement * statements = parse_statement();

        statement *whilestmt = new while_statement(expr,statements,line_number);
        return whilestmt;

}

statement* parser::  parse_conditional_statement(){

        int line_number = consume_token(IF).line_number;
        consume_token(LEFT_PAREN);
        expression *expr = parse_expression();
        consume_token(RIGHT_PAREN);

        statement* ifstatements = parse_statement();
        statement* elsestatement = NULL;

        if(match(ELSE)){

            consume_token(ELSE);
            elsestatement = parse_statement();

        }

        statement* ifstmt = new conditional_statement(expr,ifstatements,elsestatement,line_number);
        return ifstmt;
}

statement * parser:: parse_block_statement(){

    int line_number = consume_token(LEFT_BRACE).line_number;
    vector<statement*> statements;

    while(!match(RIGHT_BRACE)){

        statements.push_back(parse_declaration());
    }

    consume_token(RIGHT_BRACE);

    statement* block = new block_statement(statements,line_number);
    return block;

}


statement* parser:: parse_print_statement(){
        
    
    consume_token(PRINT);
    expression* exp = parse_expression();
    int line_number = consume_token(SEMICOLON).line_number;

    statement* ps = new print_statement(exp,line_number);
    return ps;

}


statement* parser:: parse_expression_statement(){

    expression* exp = parse_expression();
    int line_number = consume_token(SEMICOLON).line_number;

    statement *ex = new expression_statement(exp,line_number);
    return ex;
}

pair<token,token_type> parser:: parse_function_parameter(){
        
    consume_token(VAR);
    consume_token(COLON);

    pair<token,token_type> parameter;
    parameter.second = get_type(consume_token(TYPE));
    parameter.first = consume_token(tok::IDENTIFIER);

    return parameter;



}


statement* parser:: parse_function_declaration_statement(){

        int line_number = consume_token(FUN).line_number;
        auto function_name = consume_token(IDENTIFIER);
        vector<pair<token,token_type>> parameters;
        consume_token(LEFT_PAREN);
        
        if(match(VAR)){

        parameters.push_back(parse_function_parameter());
        while(!match(RIGHT_PAREN)){

            consume_token(COMMA);
            parameters.push_back(parse_function_parameter());


        }

        }
        consume_token(RIGHT_PAREN);
        consume_token(COLON);
        unordered_set<token_type> valid_types = {TYPE,VOID_TYPE};
        token_type return_type = get_type(consume_token(valid_types));
        statement* statements = parse_block_statement();

        statement* fndec_stmt = new function_declaration_statement(function_name,parameters,statements, return_type,line_number);


        return fndec_stmt;

}



statement* parser:: parse_class_method(){

        auto function_name = consume_token(IDENTIFIER);
        vector<pair<token,token_type>> parameters;
        consume_token(LEFT_PAREN);
        
        if(match(VAR)){

        parameters.push_back(parse_function_parameter());
        while(!match(RIGHT_PAREN)){

            consume_token(COMMA);
            parameters.push_back(parse_function_parameter());


        }
        }

        consume_token(RIGHT_PAREN);
        consume_token(COLON);
        token_type return_type = get_type(consume_token(TYPE));
        statement* statements = parse_block_statement();

        statement* fndec_stmt = new function_declaration_statement(function_name,parameters,statements, return_type,function_name.line_number);

        return fndec_stmt;

}
statement* parser::  parse_class_declaration_statement(){
            
        consume_token(CLASS);
        auto class_name = consume_token(IDENTIFIER);

        consume_token(tok::LEFT_BRACE);

        vector<statement*> methods;

        while(!match(tok::RIGHT_BRACE)){

            methods.push_back(parse_class_method());

        }

        consume_token(tok::RIGHT_BRACE);
        statement * class_declaration_stmt = new class_declaration_statement(class_name,methods,class_name.line_number);
        return class_declaration_stmt;

}

