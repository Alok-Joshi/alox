#include "parser.h"
#include "token.h"
#include "ast.h"

using namespace std;
using namespace tok;
using namespace ast;





parser:: parser(vector<token> &tokens){
        this->current = 0;
        this->tokens = tokens;
}




expression* parser:: parse_expression(){
        return this->parse_equality()

}


expression* parser:: parse_equality(){

    expression *left = parse_comparison();
    unordered_set<token_type> valid_operators = {BANG_EQUAL,EQUAL};

    while(match(valid_operators)){
        
        token optr = get_operator(); //will increment by current by and return operator token. put this comment in get_operator() function
        expression *right = parse_comparision();
        left = binary_expression(left,optr,right);
            
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
            expression *right = unary();
            return unary_expression(optr,right);
        }

        return parse_literal(); 

}


expression* parser:: parse_literal(){
    
    unordered_set<token_type> valid_types = {IDENTIFIER,NUMBER,STRING,TRUE,FALSE,NIL};

    if(match(valid_types)){

        expression* litexpr = new literal_expression(get_literal());
        return litexpr;
    }
    else
    {
        if(tokens[current].type == LEFT_PAREN)
        {
                current++; //consume the opening (
                expression *expr =  parse_expression(); //expression inside (expression)

                if(tokens[current] != RIGHT_PAREN)
                {
                    //error, expected ). TODO: need to dry run how this error will look like
                }

                return expr;
        }

}

bool parser:: match(unordered_set &valid_types){

        return valid_types.count(tokens[current].type);

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

