#include <iostream>
#include "cmd/cmd.h"
using namespace std;

int main(int argc, char* argv[]){
    Cmd cmd = parseCmd(argc, argv);
    return 0;
}