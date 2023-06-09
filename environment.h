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



class symbol_table {


    std:: vector<std::pair<std::string,std::map<std::string,std::any>>> scopes;
    /* Above data structure is a stack of pairs. Each pair represents a scope .
    First element of the pair is the name of the scope, 2nd element is a hash table with entries of that scope */
    
    public:
    void start_scope(std::string name);
    void end_scope();

    void add_entry(tok::token symbol, std:: any symbol_information);
    bool resolve_identifier(tok:: token identifier);
    bool is_redeclaration(tok:: token identifier);




};

#endif
