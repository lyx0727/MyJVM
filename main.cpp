#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "jvm.h"
#include <dirent.h>
#include <cstring>
#include "test/test.h"
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
        
    }

    return 0;
}