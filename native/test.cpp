#include <iostream>
#include "registry.h"
#include "../heap/class.h"
#include "../instruction/interpreter.h"
using namespace std;

void ClassTest(const string& className, bool verboseClass = false, bool verboseInst = false){
    cout << string(20, '-') << className << string(20, '-') << endl; 
    Classpath cp("jre", "src");
    ClassLoader classLoader(&cp, verboseClass);
    Class* test = classLoader.loadClass(className);    
    Method* main = test->getMainMethod();
    if(main){
        interpret(main, verboseInst);
    }
    else{
        cout << "No main method" << endl;
    }
    
}

int main(){
    init();
    // ClassTest("com/lyx/GetClassTest", true, true);
    ClassTest("com/lyx/ParseIntTest", true, true);
}