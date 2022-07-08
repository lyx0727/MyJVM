#include "cmd.h"
#include <iostream>
#include <cstring>

using namespace std;

Cmd parseCmd(int argc, char* argv[]){
    Cmd cmd;
    int i;
    for(i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            char* arg = argv[i] + 1;
            // -version
            if(strcmp(arg, "version") == 0){
                cmd.versionFlag = true;
            }
            // -help / -?
            else if(strcmp(arg, "help") == 0 || strcmp(arg, "?") == 0){
                cmd.helpFlag = true;
            }
            // -cp / -classpath
            else if(strcmp(arg, "cp") == 0 || strcmp(arg, "classpath") == 0){
                cmd.cpOption = argv[i + 1];
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    
    cmd.Class = argv[i++];
    
    for(; i < argc; i++){
        cmd.args.push_back(argv[i]);
    }

    if(cmd.versionFlag){
        cout << "version 0.0.1" << endl;
    }
    else if(cmd.helpFlag){
        printUsage(cmd);
    }
    else{
        
    }

    return cmd;
}

void printUsage(Cmd cmd){
    cout << "Usage: " << cmd.Class << " [-options] class [args...]" << endl; 
}