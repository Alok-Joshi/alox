#include "environment.h"
#include "token.h"

using namespace std;
using namespace tok;



//SYMBOL TABLE IMPLEMENTATIONS


symbol_table_entry symbol_table:: get_entry(tok:: token symbol) {


    int stack_top = this->scopes.size()-1;

    while(stack_top>=0) {


        if(this->scopes[stack_top].count(symbol.lexeme)) return this->scopes[stack_top][symbol.lexeme];
        else stack_top--;

    }


    throw "Entry not found";

}
bool symbol_table:: resolve_identifier(tok:: token identifier) {


    int stack_top = this->scopes.size()-1;

    while(stack_top>=0) {


        if(this->scopes[stack_top].count(identifier.lexeme)) return true;
        else stack_top--;

    }

    return false;
}

bool symbol_table:: is_redeclaration(tok:: token identifier) {

    return this->scopes[scopes.size()-1].count(identifier.lexeme);

}

void symbol_table:: start_scope(token name) {


    this->function_tracker.push_back(name);
    this->start_scope();

}

void symbol_table:: start_scope() {

    map<string,symbol_table_entry> new_scope;
    this->scopes.push_back(new_scope);

}

void symbol_table:: end_scope(bool is_function_scope) {

    this->scopes.pop_back();

    if(is_function_scope) {

        this->function_tracker.pop_back();

    }

}
void symbol_table:: modify_entry(token symbol, symbol_table_entry symbol_information) {
    //API specifically designed for declaration statements
    scopes[scopes.size()-1][symbol.lexeme] = symbol_information;

}



void symbol_table:: add_entry(token symbol, symbol_table_entry symbol_information) {
    //API specifically designed for declaration statements

    int top_index = scopes.size()-1;
    if(scopes[top_index].count(symbol.lexeme)) {
        //implies identifier declared before

        throw "redeclaration error";
    }
    else
    {
        scopes[top_index][symbol.lexeme] = symbol_information;
    }
}


token symbol_table:: get_current_function() {


    if(this->function_tracker.size() == 0) {

        throw "No Current Function"; //TODO: Improve this during error handling


    }

    else
    {

        int top_index = this->function_tracker.size() - 1;
        return this->function_tracker[top_index];
    }

}


//SYMBOL TABLE ENTRIES


symbol_table_entry:: symbol_table_entry(tok::token_type symbol_type): symbol_type(symbol_type) {};
symbol_table_entry:: symbol_table_entry(tok::token_type symbol_type,std::vector<std::pair<tok::token,tok::token_type>> parameters, token_type return_type):  symbol_type(symbol_type), parameters(parameters), return_type(return_type) {}
symbol_table_entry:: symbol_table_entry() {}







