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

void ClassTest(const string& className, bool verboseClass = false, bool verboseInst = false){
    Classpath cp("jre", "src");
    ClassLoader classLoader(&cp, verboseClass);
    Class* FibonacciTest = classLoader.loadClass(className);    

    Method* main = FibonacciTest->getMainMethod();
    interpret(main, verboseInst);
}

void MyObjectTest(){
    ClassTest("com/lyx/MyObject");
}

void FibonacciTest(){
    ClassTest("com/lyx/FibonacciTest");
}

void ArrayTest(){
    ClassTest("com/lyx/ArrayTest", true, true);
}

int main(){
    // MyObjectTest();
    // testDescriptorParser();
    // FibonacciTest();
    ArrayTest();

    return 0;
}