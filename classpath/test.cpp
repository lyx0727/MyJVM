#include "classpath.h"
#include <iostream>
#include <cstdint>
using namespace std;

void testDirEntry(){
    Entry* entry = getEntry("src/com/lyx");
    cout << *entry << endl;
    vector<Byte> bytes = entry->readClass("test");
    for(Byte c : bytes){
        cout << hex << (int)c << ' ';
    }
    cout << endl;
    delete entry;
}

int main(){
    testDirEntry();
    return 0;
}