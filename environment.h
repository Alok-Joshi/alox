#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include<map>
#include<any>
#include<string>
#include<vector>
#include<utility>
#include "token.h"


//singleton design pattern
class environment {

    static environment* env_variable;
    std:: vector<map<std::string,std::any>> scopes;
    public:
    static environment* get_environment();
    
    void push_scope();
    void pop_scope();
    void add_variable(tok:: token identifier, std::any value);
    void set_variable(tok::token identifier, std::any value);
    std::any get_variable(tok:: token identifier);

};


#endif
