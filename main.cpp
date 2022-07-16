#include <iostream>
#include "jvm.h"
using namespace std;

void startJVM(Cmd cmd){
    Classpath cp(cmd.XjreOption, cmd.cpOption);
    vector<Byte> bytes = cp.readClass(cmd.Class);
    cout << cp.toString() << endl;
    cout << "\n----------" << cmd.Class << "----------" << endl;
    Classfile cf(bytes);
    cout << cf.toString() << endl;
}

int main(int argc, char* argv[]){
    Cmd cmd = parseCmd(argc, argv);

    if(cmd.versionFlag){
        cout << "version 0.0.1" << endl;
    }
    else if(cmd.helpFlag){
        printUsage(argv);
    }
    else{
        startJVM(cmd);
    }

    return 0;
}