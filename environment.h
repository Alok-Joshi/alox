#include<unordered_map>
#include<any>
#include<string>
#include "token.h"


//singleton design pattern

class environment {

    static environment* env_variable;
    std:: unordered_map<tok::token,std::any> env_map;
    environment();
    static environment* get_environment();
    
    void add_variable(tok:: token identifier, std::any value);
    std::any get_variable(tok:: token identifier);
    std::string get_variable(tok:: token identifier, std::string value);

};
