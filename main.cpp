#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "jvm.h"
#include <dirent.h>
#include <cstring>
#include "test/test.h"
using namespace std;

void getFileNames(string path, vector<string>& fileNames){
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(path.c_str()))){
        cerr << "Folder doesn't exist!" << endl;
        return;
    }
    while((ptr = readdir(pDir))!=0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0){
            fileNames.push_back(path + "/" + ptr->d_name);
        }
    }
    closedir(pDir);
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
        
    }

    return 0;
}