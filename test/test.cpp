#include "test.h"
using namespace std;

void testDirEntry(){
    Entry* entry = getEntry("src/com/lyx");
    cout << *entry << endl;
    vector<Byte> bytes = entry->readClass("test");
    for(Byte c : bytes){
        cout << c;
    }
    delete entry;
}