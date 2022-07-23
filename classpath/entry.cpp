#include "entry.h"
#include <unistd.h>
#include <fstream>
#include <iostream>

using namespace std;

Entry* getEntry(const string& path){
    if(isZip(path)){
        return new ZipEntry(path);
    }
    if(path.back() == '*'){
        return new WildcardEntry(path);
    }
    return new DirEntry(path);
}

ostream& operator<<(ostream& os, const Entry& entry){
    os << entry.toString();
    return os;
}

/*
 * DirEntry
 */
DirEntry::DirEntry(const std::string& path): absDir(getAbsPath(path)) {}

vector<Byte> DirEntry::readClass(const std::string& className){
    string fileName = this->absDir + "/" + className + ".class";
    ifstream classFile;
    classFile.open(fileName, ios::in);
    vector<Byte> bytes((istreambuf_iterator<char>(classFile)), istreambuf_iterator<char>());
    return bytes;
}

/*
 * ZipEntry
 */
ZipEntry::ZipEntry(const std::string& path):absDir(getAbsPath(path)){}

vector<Byte> ZipEntry::readClass(const std::string& className){
    // TODO
    return vector<Byte>();
}

/*
 * CompositeEntry
 */
CompositeEntry::CompositeEntry(const vector<string>& paths){
    for(const string& path : paths){
        this->entrys.push_back(getEntry(path));
    }
}

const string CompositeEntry::toString() const{
    string s;
    for(Entry* entry : entrys){
        s += entry->toString() + '\n';
    }
    return s;
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

/*
 * WildcardEntry
 */
void walkFn(const string& fileName, void* that){
    WildcardEntry* This = (WildcardEntry *)that;
    if(isDir(fileName)){
        This->addEntry(new DirEntry(fileName));
    }
    else if(isZip(fileName)){
        This->addEntry(new ZipEntry(fileName));
    }
}

WildcardEntry::WildcardEntry(const string& path){
    // remove '/*'
    string absPath = getAbsPath(path);
    absPath = absPath.substr(0, absPath.length() - 2);
    walkDir(absPath, walkFn, this);
}

const string WildcardEntry::toString() const{
    string s;
    for(Entry* entry : entrys){
        s += entry->toString() + '\n';
    }
    return s;
}

vector<Byte> WildcardEntry::readClass(const string& className){
    vector<Byte> bytes;
    for(Entry* entry : entrys){
        bytes = entry->readClass(className);
        if(!bytes.empty()){
            return bytes;
        }
    }
    return bytes;
} 

WildcardEntry::~WildcardEntry(){
    for(Entry* entry : entrys){
        delete entry;
    }
}