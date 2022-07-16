#include "classfile.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(){
    ifstream in("src/com/lyx/Test.class");
    vector<Byte> bytes((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());    
    
    Classfile cf(bytes);
    cout << cf.toString() << endl;
    
    return 0;
}