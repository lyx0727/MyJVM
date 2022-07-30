#include "cmd.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>

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
                cmd.cpOption = argv[++i];
            }
            // -Xjre
            else if(strcmp(arg, "Xjre") == 0){
                cmd.XjreOption = argv[++i];
            }
            // -Xss
            else if(strcmp(arg, "Xss") == 0){
                cmd.Xss = (unsigned int)atoi(argv[++i]);
            }
            // -verbose:class / -verbose
            else if(strcmp(arg, "verbose:class") == 0 || strcmp(arg, "verbose") == 0){
                cmd.verboseClassFlag = true;
            }
            // -verbose:inst
            else if(strcmp(arg, "verbose:inst") == 0){
                cmd.verboseInstFlag = true;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }

    if(i < argc){
        cmd.Class = argv[i++];
        for(; i < argc; i++){
            cmd.args.push_back(argv[i]);
        }
    }
    else{
        printUsage(argv);
        exit(1);
    }

    return cmd;
}

void printUsage(char* argv[]){
    cout << "Usage: " << argv[0] << " [-options] class [args...]" << endl; 
}