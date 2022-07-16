#include "classpath.h"
#include <iostream>
#include <cstdint>
using namespace std;

void testDirEntry(){
    Entry* entry = getEntry("src/com/lyx");
    cout << *entry << endl;
    vector<Byte> bytes = entry->readClass("Test");
    print(bytes);
    delete entry;
}

void testWildcardEntry(){
    Entry* entry = getEntry("*");
    cout << *entry << endl;
    vector<Byte> bytes = entry->readClass("Test");
    print(bytes);
    delete entry;
}

void testClasspath(){
    Classpath cp("", "");
    cout << cp.toString() << endl;
    string className = "java.lang.Object";
    vector<Byte> bytes = cp.readClass(className);
    cout << "\n----------" << className << "----------" << endl;
    print(bytes);
}

int main(){
    cout << "/*************** TEST DirEntry ***************/" << endl;
    testDirEntry();
    cout << "/*************** TEST WildcardEntry ***************/" << endl;
    testWildcardEntry();
    cout << "/*************** TEST classpath ***************/" << endl;
    testClasspath();
    return 0;
}