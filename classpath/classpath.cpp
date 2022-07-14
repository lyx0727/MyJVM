#include "classpath.h"
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

Classpath* parse(const string& jreOption, const string& cpOption){
    Classpath* cp = new Classpath();
    cp->parseBootAndExtClasspath(jreOption);
    cp->parseUserClasspath(cpOption);
    return cp;
}

const string getJreDir(const string& jreOption){
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
    bytes = this->bootClasspath->readClass(className);
    if(!bytes.empty()){
        return bytes;
    }
    bytes = this->extClasspath->readClass(className);
    if(!bytes.empty()){
        return bytes;
    }
    return this->userClasspath->readClass(className);
}
