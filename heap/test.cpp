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
    cout << string(20, '-') << className << string(20, '-') << endl; 
    Classpath cp("jre", "src");
    ClassLoader classLoader(&cp, verboseClass);
    Class* test = classLoader.loadClass(className);    
    Method* main = test->getMainMethod();
    interpret(main, verboseInst);
}

void MyObjectTest(){
    ClassTest("com/lyx/MyObject", true);
}

void FibonacciTest(){
    ClassTest("com/lyx/FibonacciTest", true);
}

void ArrayTest(){
    ClassTest("com/lyx/ArrayTest", true, true);
}

void BubbleSortTest(){
    ClassTest("com/lyx/BubbleSortTest", true);
}

int main(){
    MyObjectTest();
    // testDescriptorParser();
    // FibonacciTest();
    // ArrayTest();
    BubbleSortTest();

    return 0;
}