#include "entry.h"
#include <unistd.h>
#include <fstream>
#include <iostream>

using namespace std;

Entry* getEntry(const string& path){
    return new DirEntry(path);
}

ostream& operator<<(ostream& os, const Entry& entry){
    os << entry.toString();
    return os;
}

DirEntry::DirEntry(const std::string& path){
    const int maxSize = 256;
    char* buf = new char[maxSize];
    this->absDir =  getcwd(buf, 256);
    this->absDir += "/" + path;
    delete[] buf;
}

vector<Byte> DirEntry::readClass(const std::string& className){
    string processedClassName;
    for(char c : className){
        if(c == '.'){
            processedClassName += '/';
        }
        else{
            processedClassName += c;
        }
    }
    string fileName = this->absDir + "/" + processedClassName + ".class";
    ifstream classFile;
    classFile.open(fileName, ios::in);
    vector<Byte> bytes((istreambuf_iterator<char>(classFile)), istreambuf_iterator<char>());
    return bytes;
}

const string& CompositeEntry::toString() const{
    string s;
    for(Entry* entry : entrys){
        s += entry->toString() + '\n';
    }
    return s;
}

CompositeEntry::CompositeEntry(const vector<string>& paths){
    for(const string& path : paths){
        this->entrys.push_back(getEntry(path));
    }
}

vector<Byte> CompositeEntry::readClass(const string& className){
    vector<Byte> bytes;
    for(Entry* entry : entrys){
        bytes = entry->readClass(className);
        if(!bytes.empty()){
            return bytes;
        }
    }
    return bytes;
} 

CompositeEntry::~CompositeEntry(){
    for(Entry* entry : entrys){
        delete entry;
    }
}