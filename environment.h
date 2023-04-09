#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include<map>
#include<any>
#include<string>
#include<utility>
#include "token.h"


//singleton design pattern

class environment {

    static environment* env_variable;
    std:: map<std::string,std::any> env_map;
    public:
    static environment* get_environment();
    
    void add_variable(tok:: token identifier, std::any value);
    std::any get_variable(tok:: token identifier);

};


#endif
