#include "scanner.h"
#include "token.h"
#include<iostream>
#include<vector>

using namespace scan;
using namespace tok;
using namespace std;

scanner:: scanner(string &source){
    this->source = source;
    this->start = 0;
    this->current = 0;
    this->line_number = 1;
    
    //initialize the token_type_identifier 

    token_type_identifier = {{"(", LEFT_PAREN},{",",COMMA},{")",RIGHT_PAREN},{"=",EQUAL},{"==",EQUAL_EQUAL},{"var",VAR},{";",SEMICOLON},{"+",PLUS},{"/",SLASH},{"-",MINUS},
                             { "{", LEFT_BRACE},{"}",RIGHT_BRACE},{".",DOT},{"*",STAR},{"!",BANG},{"!=",BANG_EQUAL},{">",GREATER},{">=",GREATER_EQUAL},{"<",LESS},
                             {"<=",LESS_EQUAL}, {"and",AND},{"class",CLASS},{"if",IF},{"nil",NIL},{"or",OR},{"print",PRINT},{"return",RETURN},{"super",SUPER},{"this",THIS},
                             {"true",TRUE},{"while",WHILE},{"else",ELSE},{"false",FALSE},{"fun",FUN},{"for",FOR},{"String",TYPE},{"Number",TYPE},{"Void",VOID_TYPE},
                             {":",COLON},{"input_number",INPUT_NUMBER}, {"input_string",INPUT_STRING}};
    //hardcoded for now. Can be generalized later
                                                                             

}


void scanner:: ignore_comments(){

        bool is_comment = check_comment_start(source);
        while(is_comment && start < source.size()){
            //to ignore the comments

            if(source[start] == '\n'){
                
                is_comment = false;
                line_number++;

            }

            start++;

        }
}


void scanner:: ignore_white_spaces() {

        while((source[start] == ' ' || source[start] == '\n') && start <source.size()){
            //to ignore the white space, end of line characters

            if(source[start] == '\n'){
                line_number++;
            }
            
            start++;

        }



}
 
vector<token> scanner:: scan_source_code(){


    while(start < source.size()){

               
        while(check_comment_start(source) || source[start] == ' ' || source[start] =='\n'){
            
            ignore_comments();
            ignore_white_spaces();
            
        }
         
        if(start == source.size()) break; //a situation where there are trailing white spaces till the end of file 

        token current_token = generate_token();
        tokens.push_back(current_token);
    }

    //push EOF character
    string eof = "\0";
    token_type eof_type = END_OF_FILE;
    tokens.push_back(token(eof,eof_type,line_number));
    return tokens;

}

bool scanner:: check_comment_start(string &source){
    if(start+2>=source.size()){
        return false;
    }
    
    else return source[start] == '/' && source[start+1] == '/';
}

pair<string,int> scanner:: get_lexeme(int start_index){
        
       int i = start_index;
       string lexeme = "";
   
       if(source[start_index] == '"'){ //check if the lexeme is a string literal 

           lexeme += source[i];
           i++;

           while( source[i] != '\0' && source[i] != '"' && source[i] != '\n'){
               
               lexeme += source[i];
               i++;
           }
           if(source[i] == '\0' ||  source[i] == '\n'){
               //throw exception of unterminated string
               throw "ERROR: Unterminated string at line number "+ to_string(line_number) + "\n";
            }
           else
           {
               lexeme += source[i];      
               return make_pair(lexeme,line_number);
           }

        }

       else if(source[start_index]>=48 && source[start_index]<=57){

                bool dot_spotted = false;
                while((source[i] == '.' || (!is_single_character_lexeme((source[i])))  && source[i] != ' ' && source[i] != '\n' && source[i] != '\0')){
                //i want to stop at all single character lexemes except the dot(.). Hence i take advantage of shortcircuiting, (if first condiiton is true, 
                //2nd is never checked. if 1st is false, 2nd is false anyways.
                    if(source[i] == '.'){
                        if(dot_spotted){

                            break; //TODO: error handling at tokenizer phase or parsing phase

                        }
                        else
                        {
                            dot_spotted = true;
                        }
                    }
                    lexeme += source[i];
                    i++;

                }

                return make_pair(lexeme,line_number);
                 
       }
           
       else if(is_single_character_lexeme(source[start_index])){ // check if we have a single character lexeme
           //includes: ( ) { } , . ; \0 > < = *,+,- etc
           if(is_two_character_operator(source.substr(start_index,2))){
                lexeme += source.substr(start_index,2);
           }
           else
           {
                lexeme += source[start_index];
           }

           return make_pair(lexeme,line_number);
       }
       while(!is_single_character_lexeme(source[i]) && source[i]!= ' ' && source[i] != '\n' && source[i] != '\0'){ //check for other lexemes (keywords, identifiers)
           
                 lexeme += source[i];
                 i++;

        }

        return make_pair(lexeme,line_number);
}
bool scanner:: is_two_character_operator(string optr){
/* checks if the given string is ==, >=, <=, != .checks the token type table if the optr is present in table. Optr is guaranteed to start with a character which can be categorised as a single character lexeme */
            bool is_tco = token_type_identifier.count(optr);
            return is_tco;
}
bool scanner:: is_single_character_lexeme(char character){

    string strch = "";
    strch += character;

    return token_type_identifier.count(strch); //the size of strch is one. the .count() will basically check if there is any token type which corresponds to strch. If there is not any, the single character lexeme  is not a single character token. 
}
token_type scanner:: identify_token(string &lexeme){

    token_type lexeme_type;

    if(token_type_identifier.count(lexeme)){
        lexeme_type =  token_type_identifier[lexeme];
    }
    else if(is_string_literal(lexeme)){
        lexeme_type = tok::STRING_TYPE;
    }
    else if(is_number_literal(lexeme)){

        lexeme_type = tok::NUMBER_TYPE;
    }
    else
    {
        lexeme_type = IDENTIFIER;
    }

    return lexeme_type;
}

bool scanner:: is_number_literal(string &lexeme){

    for(int i = 0; i<lexeme.size(); i++){
        
        int dotcount = 0;
        if((dotcount<=1 && lexeme[i] == '.') || lexeme[i]>=48 && lexeme[i]<=57){
        
            if(lexeme[i] == '.') dotcount++;
            continue;
        }
        else return false;
    }

    return true;
}

bool scanner:: is_string_literal(string &lexeme){

       return lexeme[0] == '"' && lexeme[lexeme.size()-1] == '"';
}


token scanner:: generate_token(){
    /*token generation has the following steps:
     * 1) extracting the lexeme 
     * 2) finding the appropriate classification type
     * 3) extracting the value
     * 4) creating the token object, returning the token object
     */
        auto lex_line = get_lexeme(start);
        auto &lexeme = lex_line.first;
        auto &line_number = lex_line.second;

        token_type lexeme_type = identify_token(lexeme);
        start += lexeme.size();

        if(lexeme_type == tok::STRING_TYPE){

            string literal_value = lexeme.substr(1,lexeme.size()-2); //need to test this
            return token(lexeme,lexeme_type,line_number,literal_value);
        }
        else if(lexeme_type == tok::NUMBER_TYPE){
            
             double literal_value = stod(lexeme);
             return token(lexeme,lexeme_type,line_number,literal_value);

        }
        else
        {
           return token(lexeme,lexeme_type,line_number); 
        }


}
