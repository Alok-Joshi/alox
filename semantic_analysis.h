#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H


#include "ast.h"
#include "environment.h"
#include <vector>

class semantic_analyser {

        
    std::vector<ast:: statement*> ast;
    environment* env;
    bool check_scope(ast:: statement* ast);
    bool check_scope(ast::expression* exp);
    bool block_resolver(ast::block_statement* block);
    public:
    semantic_analyser(std:: vector<ast:: statement*> ast);
    bool check_scope(); //a switch statemene

     


};





#endif


