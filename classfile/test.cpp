#include <iostream>
#include <fstream>
#include "classfile.h"
using namespace std;
using namespace classfile;

int main(){
    // ifstream in("src/com/lyx/Test.class");
    ifstream in("jre/lib/rt/java/lang/String.class");
    vector<Byte> bytes((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());    
    Classfile cf(bytes);
    
    cout << cf.toString() << endl;
    return 0;
}