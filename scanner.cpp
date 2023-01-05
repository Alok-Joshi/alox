#include "scanner.h"
#include "token.h"

scanner:: scanner(string &source){
    this->source = source;
    this->start = 0;
    this->current = 0;
    this->line_number = 0;
    
    //initialize the token_type_identifier 

    token_type_identifier = {{"(", LEFT_PAREN},{")",RIGHT_PAREN},{"=",EQUAL},{"==",EQUAL_EQUAL},{}};
    //TODO: complete the above unordered_map
}

vector<token> scanner::  scan_source_code(){

    bool is_comment = false;
    //TODO: Decide a design for resetting the scanner for taking in more tokens. (could be useful for command line like interpreters)

    while(start < source.size()){
        
        is_comment = check_comment_start(source);

        if(is_comment || source[start] == ' ' || source[start] == '\n'){
            //to ignore the white space, end of line characters, and comments

            if(source[i] == '\n'){
                line_number++;
            }
            if(is_comment && source[start] == '\n'){   //implies comment has ended  

              is_comment = false;
            }
            
            start++;
            continue;
        }

        token current_token = generate_token();
        tokens.push_back(token);
    }

    //push EOF character
    tokens.push_back(token("\0",EOF,line_number));
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

       if(is_single_character_lexeme(source[start_index])){ //check this here itself to avoid misinterpreting the lexemes.
           //includes: ( ) { } , . ; \0 > < = *
           lexeme += source[start_index];
           return make_pair(lexeme,line_number);
       }

       while(!is_single_character_lexeme(source[i]) && source[i]!= ' ' && source[i] != '\n'){

                 lexeme += source[i];
                 i++;

        }
        

        return make_pair(lexeme,line_number);
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
        lexeme_type = STRING;
    }
    else if(is_number_literal(lexeme)){
        lexeme_type = NUMBER;
    }

    return lexeme_type;
}

bool scanner:: is_number_literal(string &lexeme){

    for(int i = 0; i<lexeme.size(); i++){

        if(lexeme[i]<48 || lexeme[i]>57){
            return false;
        }
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
        auto lexeme = get_lexeme(start_index);
        token_type lexeme_type = identify_token(lexeme.first);
        start_index+= lexeme.size();

        if(token_type == STRING){

            string literal_value = lexeme.substr(1,lexeme.size()-2); //need to test this
            return token(lexeme,lexeme_type,lexeme.second,literal_value);
        }
        else if(token_type == NUMBER){
            
             int literal_value = stoi(lexeme);
             return token(lexeme,lexeme_type,lexeme.second,literal_value);

        }
        else
        {
           return token(lexeme,lexeme_type,lexeme.second); 
        }


}
