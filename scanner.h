//scanner.h
#ifndef SCANNER_H
#define SCANNER_H

#include<string>
#include<vector>
#include<unordered_map>
#include "token.h"

namespace scan {
class scanner{
    std:: string source;
    std::vector<tok::token> tokens;
    std::unordered_map<std::string,tok::token_type> token_type_identifier;

    int start; //indicates the start position of the current lexeme
    int current; //indicate the current position in the source code
    int line_number; //indicates the current line in the source code

    public:
    scanner(std::string &source);
    std::vector<tok::token> scan_source_code();
    tok::token generate_token();
    std::string generate_lexeme(int start_index);
    bool is_string_literal(std::string &lexeme);
    bool is_number_literal(std::string &lexeme);
    tok::token_type identify_token(std::string &lexeme);
    bool is_single_character_lexeme(char character);
    std::pair<std::string,int>  get_lexeme(int start_index);
    bool check_comment_start(std::string &source);
    


};


}

#endif
