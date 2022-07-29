#include <iostream>
#include <cmath>
#include <fstream>
#include "instruction.h"
#include "interpreter.h"
#include "../classfile/classfile.h" 
#include <cmath>
#include "math.h"
using namespace std;
using namespace classfile;

void GaussTest(){
    ifstream in("src/com/lyx/GaussTest.class");
    vector<Byte> bytes((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());     
    Classfile cf(bytes);
    MemberInfo* main = cf.getMainMethod();  
    interpret(main, false);
}

int main(){
    GaussTest();
    return 0;
}