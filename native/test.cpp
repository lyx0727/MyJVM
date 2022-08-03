#include <iostream>
#include "java/lang/Class.h"
#include "java/lang/Object.h"
#include "../heap/class.h"
#include "../instruction/interpreter.h"
using namespace std;

void ClassTest(const string& className, bool verboseClass = false, bool verboseInst = false){
    cout << string(20, '-') << className << string(20, '-') << endl; 
    Classpath cp("jre", "src");
    ClassLoader classLoader(&cp, verboseClass);
    Class* test = classLoader.loadClass(className);    
    Method* main = test->getMainMethod();
    interpret(main, verboseInst);
}

int main(){
    java_lang_Object::init();
    java_lang_Class::init();
    ClassTest("com.lyx.GetClassTest");
}