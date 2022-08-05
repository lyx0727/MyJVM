#include <iostream>
#include "jvm.h"
using namespace std;

void startJVM(Cmd cmd){
    init();
    Classpath cp(cmd.XjreOption, cmd.cpOption);
    ClassLoader classLoader(&cp, cmd.verboseClassFlag);
    string className = replace(cmd.Class, '.', '/');
    Class* _class = classLoader.loadClass(className);    
    Method* main = _class->getMainMethod();
    if(main == nullptr){
        cout << "Main method not found in class " << _class->name << endl;
    }
    else{
        interpret(main, cmd.verboseInstFlag, cmd.args);
    }
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