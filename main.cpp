#include "scanner.h"
#include<iostream>


using namespace scan;
using namespace std;




int main(){

    string source = "var a = -1232";

    try {
    scanner s(source);
    auto a = s.scan_source_code();

    for(auto &el: a){
        el.print_token();
        cout<<endl;
    }
    }
    catch (const char* msg){
        cout<<msg<<endl;
    }
    
    return 0;


}
