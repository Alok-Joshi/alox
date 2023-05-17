#include "scanner.h"
#include "parser.h"
#include "interpreter.h"
#include <any>
#include<iostream>
#include <memory>
#include <fstream>


using namespace scan;
using namespace std;
using namespace ast;


int main(int argc, char** argv){

    ifstream source_code;
    source_code.open(argv[1],ios::in);

    string source = "";

    string temp = "";

    while(getline(source_code,temp)){

        source += temp;
    }

    source_code.close();

    scanner scan(source);
    auto tokens = scan.scan_source_code();
    parser p(tokens);

    auto tree = p.parse_program();
    interpreter inp(tree);
    inp.interpret();



    return 0;


    
}
