#include "parser.h"
#include "environment.h"
#include "token.h"
#include "ast.h"
#include <iostream>
#include <unordered_set>
#include <memory>

using namespace std;
using namespace tok;
using namespace ast;




token_type get_type(token type_word) {


    if(type_word.lexeme == "Void") return tok::VOID_TYPE;
    if(type_word.lexeme == "Number") return tok::NUMBER_TYPE;
    if(type_word.lexeme == "String") return tok::STRING_TYPE;

}

parser:: parser(vector<token> &tokens) {
    this->current = 0;
    this->tokens = tokens;
}




std::unique_ptr<expression> parser:: parse_expression() {
    return this->parse_assignment();

}
std::unique_ptr<expression> parser :: parse_assignment() {


    unique_ptr<expression> left = parse_logical_or();

    if(typeid((*left)) == typeid(variable_literal_expression)) {


        if(match(EQUAL)) {

            auto tok = consume_token(EQUAL);
            unique_ptr<expression> expr = parse_assignment();
            left = std::make_unique<binary_expression>(std::move(left),tok,std::move(expr),tok.line_number);
            return left;

        }

    }

    return left;


}

std::unique_ptr<expression> parser:: parse_logical_or() {

    unique_ptr<expression> left = parse_logical_and();

    while(match(OR)) {

        auto tok = consume_token(OR);
        unique_ptr<expression> expr = parse_logical_and();
        left = make_unique<logical_expression>(std::move(left),tok,std::move(expr),tok.line_number);

    }


    return left;



}

std::unique_ptr<expression> parser:: parse_logical_and() {

    std::unique_ptr<expression> left = parse_equality();

    while(match(AND)) {

        auto tok = consume_token(AND);
        std::unique_ptr<expression> expr = parse_equality();
        left = std::make_unique<logical_expression>(std::move(left),tok,std::move(expr),tok.line_number);

    }

    return left;

}



std::unique_ptr<expression> parser:: parse_equality() {


    std::unique_ptr<expression> left = parse_comparison();
    unordered_set<token_type> valid_operators = {BANG_EQUAL,EQUAL_EQUAL};

    while(match(valid_operators)) {

        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        std::unique_ptr<expression> right = parse_comparison();
        left = std::make_unique<binary_expression>(std::move(left),optr,std::move(right),optr.line_number);

    }

    return left;

}


std::unique_ptr<expression> parser:: parse_comparison() {
    std::unique_ptr<expression> left = parse_addsub();
    unordered_set<token_type> valid_operators = {GREATER,GREATER_EQUAL,LESS,LESS_EQUAL,EQUAL_EQUAL};


    while(match(valid_operators)) {

        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        std::unique_ptr<expression> right = parse_addsub();
        left = std::make_unique<binary_expression>(std::move(left),optr,std::move(right),optr.line_number);

    }

    return left;

}

std::unique_ptr<expression> parser:: parse_addsub() {

    std::unique_ptr<expression> left = parse_multdiv();
    unordered_set<token_type> valid_operators = {PLUS,MINUS};


    while(match(valid_operators)) {

        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        std::unique_ptr<expression> right = parse_multdiv();
        left = std::make_unique<binary_expression>(std::move(left),optr,std::move(right),optr.line_number);

    }

    return left;

}

std::unique_ptr<expression> parser:: parse_multdiv() {

    std::unique_ptr<expression> left = parse_unary();
    unordered_set<token_type> valid_operators = {STAR,SLASH};

    while(match(valid_operators)) {

        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        std::unique_ptr<expression> right = parse_unary();
        left = std::make_unique<binary_expression>(std::move(left),optr,std::move(right),optr.line_number);

    }

    return left;


}

std::unique_ptr<expression> parser:: parse_unary() {

    unordered_set<token_type> valid_types = {BANG,MINUS};
    while(match(valid_types)) {
        token optr = get_operator();
        std::unique_ptr<expression> right = parse_unary();
        return std::make_unique<unary_expression>(optr,std::move(right),optr.line_number);
    }

    return parse_call();

}

std::unique_ptr<expression> parser:: parse_call() {


    std::unique_ptr<expression> exp = parse_literal();
    vector<std::unique_ptr<expression>> arguments;

    int line_number = -1;

    if(typeid((*exp)) == typeid(variable_literal_expression)) {


        if(match(LEFT_PAREN)) {
            consume_token(LEFT_PAREN); //will be later replaced by outer while loop for multiple calls

            if(!match(RIGHT_PAREN)) { //implies there is an argument given

                arguments.push_back(parse_expression());
                while(!match(RIGHT_PAREN)) {

                    consume_token(COMMA);
                    arguments.push_back(parse_expression());


                }

            }

            line_number = consume_token(RIGHT_PAREN).line_number;

        }
    }

    if(line_number != -1) {
        tok::token token_name = static_cast<variable_literal_expression*>(exp.get())->get_variable_name();
        std::unique_ptr<expression> call_expr = std::make_unique<function_call_expression>(token_name,std::move(arguments),line_number);
        return call_expr;

    }
    else
    {

        return exp;

    }

}
std::unique_ptr<expression> parser:: parse_literal() {

    unordered_set<token_type> valid_types = {IDENTIFIER,NUMBER_TYPE,STRING_TYPE,TRUE,FALSE,NIL};

    if(match(valid_types)) {

        token literal_obj = get_literal();
        if(literal_obj.type == IDENTIFIER) {
            std::unique_ptr<expression> litvarexp = std::make_unique<variable_literal_expression>(literal_obj,literal_obj.line_number);
            return litvarexp;

        }
        else
        {
            std::unique_ptr<expression> litexpr = std::make_unique<literal_expression>(literal_obj,literal_obj.line_number);
            return litexpr;

        }
    }
    else if(tokens[current].type == LEFT_PAREN)
    {
        current++; //consume the opening (
        std::unique_ptr<expression> expr =  parse_expression(); //expression inside (expression)

        if(tokens[current].type != RIGHT_PAREN)
        {
            //error, expected ). TODO: need to dry run how this error will look like
        }

        return expr;
    }
    else
    {
        int current_line_number  = tokens[current].line_number;
        string error = "ERROR at line " +to_string(current_line_number) + ": Expected expression" ;
        throw  error;

    }
}

bool parser:: match(unordered_set<token_type> &valid_types) {

    return valid_types.count(tokens[current].type);

}
bool parser :: match(token_type tt) {

    return tokens[current].type == tt;
}

token parser:: get_operator() {

    token optr = tokens[current];
    ++current;
    return optr;
}

token parser:: get_literal() {
    token literal = tokens[current];
    ++current;
    return literal;
}


token parser:: consume_token(unordered_set<token_type> &valid_types) {

    token literal = tokens[current];
    if(valid_types.count(literal.type)) {

        current++;
        return literal;
    }
    else
    {
        string error = "ERROR at line " +to_string(literal.line_number) + " :  Expected token \"" + "\"";
        throw  error;
    }

}
token parser:: peak() {
    //simply returns the current token, does not consume it (current remains where it is)
    return tokens[current];

}
token parser:: consume_token(token_type valid_type) {

    token tk = tokens[current];
    if(valid_type == tk.type) {

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

std::vector<std::unique_ptr<statement>> parser:: parse_program() {

    vector<std::unique_ptr<statement>> declarations;

    while(!match(END_OF_FILE)) {

        declarations.push_back(parse_declaration());

    }

    consume_token(END_OF_FILE);

    return declarations;

}
std::unique_ptr<statement> parser:: parse_return_statement() {


    int line_number = consume_token(RETURN).line_number;

    std::unique_ptr<expression> expr = parse_expression();
    consume_token(SEMICOLON);

    std::unique_ptr<return_statement> rtsmt = std::make_unique<return_statement>(std::move(expr),line_number);
    return rtsmt;


}

void parser:: synchronise() {


    while(!match(tok::END_OF_FILE)) {

        if(tokens[current-1].type == SEMICOLON) return;

        unordered_set<token_type> statement_start = {FUN,WHILE,FOR,VAR,CLASS,INPUT_NUMBER,INPUT_STRING,PRINT,IF,RETURN,INPUT_NUMBER,INPUT_STRING};
        if(statement_start.count(tokens[current].type)) return;

        current++;

    }

}
std::unique_ptr<statement> parser:: parse_declaration() {


    try {

        if(match(VAR)) { //declarations start with VAR

            int line_number = consume_token(VAR).line_number;
            consume_token(COLON);
            token_type variable_type = get_type(consume_token(TYPE));

            auto variable_name = peak();
            std::unique_ptr<expression> exp = parse_expression();

            std::unique_ptr<statement> declaration_stmt = std::make_unique<declaration_statement>(std::move(exp),variable_name,variable_type,line_number);

            consume_token(SEMICOLON);

            return declaration_stmt;


        }
        else if(match(FUN)) {

            return parse_function_declaration_statement();

        }
        else if(match(CLASS)) {

            return parse_class_declaration_statement();

        }
        else //its a different kind of statement, so direct to generic statement
        {

            return parse_statement();

        }
    }
    catch(string error) {

        cout<<error<<endl;
        this->synchronise();
        error_status = true;

    }
}


std::unique_ptr<statement> parser:: parse_statement() {

    if(match(PRINT)) {


        return parse_print_statement();

    }
    else if(match(LEFT_BRACE)) {

        return parse_block_statement();

    }
    else if(match(IF)) {


        return parse_conditional_statement();

    }
    else if(match(WHILE)) {


        return parse_while_statement();
    }
    else if(match(FOR)) {


        return parse_for_statement();
    }
    else if(match(RETURN)) {

        return parse_return_statement();

    }

    else if(match(tok::INPUT_STRING)) {


        return parse_string_input_statement();

    }

    else if(match(INPUT_NUMBER)) {

        return parse_number_input_statement();

    }

    else {

        //its an expression statement. this will change as we add more statement types
        return parse_expression_statement();


    }

}

std::unique_ptr<statement> parser:: parse_number_input_statement() {


    consume_token(tok::INPUT_NUMBER);
    token input_reciever = consume_token(tok::IDENTIFIER);
    consume_token(tok::SEMICOLON);

    std::unique_ptr<input_statement> number_input_stmt = std::make_unique<input_statement>(input_reciever,tok::NUMBER_TYPE,input_reciever.line_number);
    return number_input_stmt;


}

std::unique_ptr<statement> parser:: parse_string_input_statement() {


    consume_token(tok::INPUT_STRING);
    token input_reciever = consume_token(tok::IDENTIFIER);
    consume_token(tok::SEMICOLON);

    std::unique_ptr<input_statement> str_input_stmt = std::make_unique<input_statement>(input_reciever,tok::STRING_TYPE,input_reciever.line_number);
    return str_input_stmt;


}

std::unique_ptr<statement> parser:: parse_for_statement() {


    consume_token(FOR);
    consume_token(LEFT_PAREN);
    std::unique_ptr<statement> part1;
    std::unique_ptr<expression> part2;
    std::unique_ptr<expression> part3;
    std::unique_ptr<statement> statements;

    if(peak().type != SEMICOLON) {
        //TODO: make an empty statement class for empty semicolons
        part1 = match(VAR)? parse_declaration(): parse_expression_statement();
    }

    if(peak().type != SEMICOLON) {

        //implies there is a condition

        part2 = parse_expression();
    }
    consume_token(SEMICOLON);

    if(peak().type != RIGHT_PAREN) {


        part3 = parse_expression();

    }


    int line_number = consume_token(RIGHT_PAREN).line_number;
    statements = parse_statement();

    std::unique_ptr<statement> for_stmt = std::make_unique<for_statement>(std::move(part1),std::move(part2),std::move(part3),std::move(statements),line_number);
    return for_stmt;

}
std::unique_ptr<statement> parser:: parse_while_statement() {

    consume_token(WHILE);
    consume_token(LEFT_PAREN);
    std::unique_ptr<expression> expr = parse_expression();
    int line_number = consume_token(RIGHT_PAREN).line_number;

    std::unique_ptr<statement> statements = parse_statement();

    std::unique_ptr<statement> whilestmt = std::make_unique<while_statement>(std::move(expr),std::move(statements),line_number);
    return whilestmt;

}

std::unique_ptr<statement> parser::  parse_conditional_statement() {

    int line_number = consume_token(IF).line_number;
    consume_token(LEFT_PAREN);
    std::unique_ptr<expression> expr = parse_expression();
    consume_token(RIGHT_PAREN);

    std::unique_ptr<statement> ifstatements = parse_statement();
    std::unique_ptr<statement> elsestatement = NULL;

    if(match(ELSE)) {

        consume_token(ELSE);
        elsestatement = parse_statement();

    }

    std::unique_ptr<statement> ifstmt = std::make_unique<conditional_statement>(std::move(expr),std::move(ifstatements),std::move(elsestatement),line_number);
    return ifstmt;
}

std::unique_ptr<statement> parser:: parse_block_statement() {

    int line_number = consume_token(LEFT_BRACE).line_number;
    vector<std::unique_ptr<statement>> statements;

    while(!match(RIGHT_BRACE)) {

        statements.push_back(parse_declaration());
    }

    consume_token(RIGHT_BRACE);

    std::unique_ptr<statement> block = std::make_unique<block_statement>(std::move(statements),line_number);
    return block;

}


std::unique_ptr<statement> parser:: parse_print_statement() {


    consume_token(PRINT);
    std::unique_ptr<expression> exp = parse_expression();
    int line_number = consume_token(SEMICOLON).line_number;

    std::unique_ptr<statement> ps = std::make_unique<print_statement>(std::move(exp),line_number);
    return ps;

}


std::unique_ptr<statement> parser:: parse_expression_statement() {

    std::unique_ptr<expression> exp = parse_expression();
    int line_number = consume_token(SEMICOLON).line_number;

    std::unique_ptr<statement> ex = std::make_unique<expression_statement>(std::move(exp),line_number);
    return ex;
}

pair<token,token_type> parser:: parse_function_parameter() {

    consume_token(VAR);
    consume_token(COLON);

    pair<token,token_type> parameter;
    parameter.second = get_type(consume_token(TYPE));
    parameter.first = consume_token(tok::IDENTIFIER);

    return parameter;



}


std::unique_ptr<statement> parser:: parse_function_declaration_statement() {

    int line_number = consume_token(FUN).line_number;
    auto function_name = consume_token(IDENTIFIER);
    vector<pair<token,token_type>> parameters;
    consume_token(LEFT_PAREN);

    if(match(VAR)) {

        parameters.push_back(parse_function_parameter());
        while(!match(RIGHT_PAREN)) {

            consume_token(COMMA);
            parameters.push_back(parse_function_parameter());


        }

    }
    consume_token(RIGHT_PAREN);
    consume_token(COLON);
    unordered_set<token_type> valid_types = {TYPE,VOID_TYPE};
    token_type return_type = get_type(consume_token(valid_types));
    std::unique_ptr<statement> statements = parse_block_statement();

    std::unique_ptr<statement> fndec_stmt = std::make_unique<function_declaration_statement>(function_name,parameters,std::move(statements), return_type,line_number);


    return fndec_stmt;

}



std::unique_ptr<statement> parser:: parse_class_method() {

    auto function_name = consume_token(IDENTIFIER);
    vector<pair<token,token_type>> parameters;
    consume_token(LEFT_PAREN);

    if(match(VAR)) {

        parameters.push_back(parse_function_parameter());
        while(!match(RIGHT_PAREN)) {

            consume_token(COMMA);
            parameters.push_back(parse_function_parameter());


        }
    }

    consume_token(RIGHT_PAREN);
    consume_token(COLON);
    token_type return_type = get_type(consume_token(TYPE));
    std::unique_ptr<statement> statements = parse_block_statement();

    std::unique_ptr<statement> fndec_stmt = std::make_unique<function_declaration_statement>(function_name,parameters,std::move(statements), return_type,function_name.line_number);

    return fndec_stmt;

}
std::unique_ptr<statement> parser::  parse_class_declaration_statement() {

    consume_token(CLASS);
    auto class_name = consume_token(IDENTIFIER);

    consume_token(tok::LEFT_BRACE);

    vector<unique_ptr<statement>> methods;

    while(!match(tok::RIGHT_BRACE)) {

        methods.push_back(parse_class_method());

    }

    consume_token(tok::RIGHT_BRACE);
    std::unique_ptr<statement> class_declaration_stmt = std::make_unique<class_declaration_statement>(class_name,std::move(methods),class_name.line_number);
    return class_declaration_stmt;

}

