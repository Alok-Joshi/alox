#include "ast.h"
#include "scanner.h"
#include "parser.h"
#include <any>
#include<iostream>
#include <memory>
#include <fstream>


using namespace scan;
using namespace std;
using namespace ast;


void bruh(int a) {}

int main(int argc, char** argv) {

    ifstream source_code;
    source_code.open(argv[1],ios::in);
    string source = "";
    string temp = "";
    while(getline(source_code,temp)) {
        source += temp;
        source += '\n';
    }
    source_code.close();
    scanner scan(source);
    auto tokens = scan.scan_source_code();
    parser p(tokens);
    auto tree = p.parse_program();
    if(p.error_status == false) {

        semantic_analyser sa(std::move(tree));
        sa.analyse_program();

        if(!sa.error_stack.empty()) {

            for(auto &at: sa.error_stack) {
                cout<<at<<endl;
            }
        }
    }


    return 0;
}
