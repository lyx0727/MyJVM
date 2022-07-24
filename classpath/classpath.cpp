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

void Classpath::parseUserClasspath(const std::string& cpOption){ this->userClasspath = getEntry(cpOption); }

pair<vector<Byte>, Entry*> Classpath::readClass(const string& className){
    vector<Byte> bytes;
    string cls = replace(className, '.', '/');
    auto p = this->bootClasspath->readClass(cls);
    if(!p.first.empty()){
        return p;
    }
    p = this->extClasspath->readClass(cls);
    if(!p.first.empty()){
        return p;
    }
    return this->userClasspath->readClass(cls);
}
