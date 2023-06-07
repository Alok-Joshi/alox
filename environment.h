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
    void add_variable(tok::token identifier);
    void set_variable(tok::token identifier, std::any value);
    bool resolve_variable(tok:: token identifier);
    bool is_redeclaration(tok:: token identifier);
    
    std::any get_variable(tok:: token identifier);

};


#endif
