#include "cmd.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    Cmd cmd = parseCmd(argc, argv);

    if(cmd.versionFlag){
        cout << "version 0.0.1" << endl;
    }
    else if(cmd.helpFlag){
        printUsage(argv);
    }
    else{
        cout << "----------[class]----------" << endl;
        cout << cmd.Class << endl;
        cout << "----------[args]----------" << endl;
        for(string& arg : cmd.args){
            cout << arg << endl;
        }
    }

    return 0;
}