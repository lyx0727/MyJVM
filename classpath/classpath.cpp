#include "classpath.h"
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

const string getJreDir(const string& jreOption){
    // TODO
    // fix jre to test
    return "jre";
    
    if(jreOption != ""){
        return jreOption;
    }
    string jh = getenv("JAVA_HOME");
    if(jh == ""){
        cerr << "Can not find jre folder!";
        exit(0);
    }
    return jh + "/jre";
}

void Classpath::parseBootAndExtClasspath(const std::string& jreOption){
    string jreDir = getJreDir(jreOption);
    // jre/lib/*
    this->bootClasspath = getEntry(jreDir + "/lib/*");
    //jre/lib/ext/*
    this->extClasspath = getEntry(jreDir + "/lib/ext/*");
}

void Classpath::parseUserClasspath(const std::string& cpOption){
    this->userClasspath = getEntry(cpOption);
}

vector<Byte> Classpath::readClass(const string& className){
    vector<Byte> bytes;
    string cls = replace(className, '.', '/');
    bytes = this->bootClasspath->readClass(cls);
    if(!bytes.empty()){
        return bytes;
    }
    bytes = this->extClasspath->readClass(cls);
    if(!bytes.empty()){
        return bytes;
    }
    return this->userClasspath->readClass(cls);
}
