#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include<map>
#include<any>
#include<string>
#include<vector>
#include<utility>
#include "token.h"


//was singleton design pattern. will change soon
class environment {

    static environment* env_variable;
    std:: vector<std::map<std::string,std::any>> scopes;
    public:
    static environment* get_environment();
    
    void push_scope();
    void pop_scope();
    void add_variable(tok:: token identifier, std::any value);
    void set_variable(tok::token identifier, std::any value);
    
    std::any get_variable(tok:: token identifier);

};




class  symbol_table_entry {

    /* single symbol table entry class for all types of entries: currently only functions and variables */

    public:
    tok::token_type symbol_type;
    tok::token_type return_type;
    std::vector<std::pair<tok::token,tok::token_type>> parameters;
    symbol_table_entry(); //for the map of scopes
    symbol_table_entry(tok::token_type symbol_type); //for variables
    symbol_table_entry(tok::token_type symbol_type, std::vector<std::pair<tok::token,tok::token_type>> parameters, tok::token_type return_type); //for functions

};

class symbol_table {


    std:: vector<std::map<std::string,symbol_table_entry>> scopes;

    /* Above data structure is a stack of pairs. Each pair represents a scope .
    First element of the pair is the name of the scope, 2nd element is a hash table with entries of that scope */

    std:: vector<tok::token> function_tracker;
    
    /*Above data structure is a stack of function names. It is used to track which function we are in currently during the semantic analysis phase. */
    
    public:
    void start_scope(tok::token function_name);
    void start_scope();
    void end_scope(bool is_function_scope = false);

    void add_entry(tok::token symbol, symbol_table_entry symbol_information);
    void modify_entry(tok::token symbol, symbol_table_entry symbol_information);
    tok::token get_current_function();
    symbol_table_entry get_entry(tok:: token symbol);
    bool resolve_identifier(tok:: token identifier);
    bool is_redeclaration(tok:: token identifier);




};

#endif
