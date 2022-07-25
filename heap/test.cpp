#include <iostream>
#include <fstream>
#include "class.h"
#include "../instruction/interpreter.h"
using namespace std;
using namespace classfile;

void testDescriptorParser(){
    MethodDescriptor md = MethodDescriptorParser("(IDLjava/lang/Thread;)Ljava/lang/Object;").parse();
    cout << md.returnType << endl;
    for(string& type : md.parameterTypes){
        cout << "\t" << type << endl;
    }
}

void MyObjectTest(){
    Classpath cp("jre", "src");
    ClassLoader classLoader(&cp);
    Class* myObject = classLoader.loadClass("com/lyx/MyObject");    

    Method* main = myObject->getMainMethod();
    interpret(main);
}

int main(){
    // MyObjectTest();
    testDescriptorParser();
    return 0;
}