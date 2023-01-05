//scanner.h
#ifndef SCANNER_H
#define SCANNER_H



class scanner{
    string source;
    vector<token> tokens;
    unordered_map<string,token_type> token_type_identifier;

    int start; //indicates the start position of the current lexeme
    int current; //indicate the current position in the source code
    int line_number; //indicates the current line in the source code

    public:
    scanner(string &source);
    vector<token> scan_source_code();
    token generate_token(int start_index);
    string generate_lexeme(int start_index);


}
