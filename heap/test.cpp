#include <iostream>
#include <fstream>
#include "class.h"
#include "../instruction/interpreter.h"
using namespace std;
using namespace classfile;

void MyObjectTest(){
    Classpath cp("jre", "src");
    ClassLoader classLoader(&cp);
    Class* myObject = classLoader.loadClass("com/lyx/MyObject");    

    Method* main = myObject->getMainMethod();
    interpret(main);
}

int main(){
    MyObjectTest();

    return 0;
}