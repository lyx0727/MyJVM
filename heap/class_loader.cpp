#include <iostream>
#include <exception>
#include "class_loader.h"
using namespace std;

ClassLoader::~ClassLoader(){
    for(auto it : classMap){
        delete it.second;
    }
}

Class* ClassLoader::loadClass(const string& name){
    if(classMap.count(name)){
        return classMap.at(name);
    }
    if(name[0] == '[') return loadArrayClass(name);
    return loadNonArrayClass(name);
}

pair<vector<Byte>, Entry*> ClassLoader::readClass(const std::string& name) {
    pair<vector<Byte>, Entry*> p = cp->readClass(name);
    if(p.first.size() == 0){
        throw JavaLangClassNotFoundException(name, __FILE__, __LINE__);
    }
    return p;
}

Class* ClassLoader::parseClass(const std::vector<Byte>& data){
    classfile::Classfile cf(data);
    return new Class(cf);
}

Class* ClassLoader::defineClass(const std::vector<Byte>& data){
    Class* _class = parseClass(data);
    _class->classLoader = this;
    _class->resolveSuperClass();
    _class->resolveInterfaces();
    classMap[_class->name] = _class;
    return _class;
}

Class* ClassLoader::loadNonArrayClass(const string& name){
    pair<vector<Byte>, Entry*> p = readClass(name);
    vector<Byte>& data = p.first;
    Entry* entry = p.second;
    Class* _class = defineClass(data);
    _class->link();
    if(verboseFlag){
        cout << "[Loaded " << name << " from " << entry->toString() << "]\n";
    }
    return _class;
}

Class* ClassLoader::loadArrayClass(const string& name){
    Class* _class = new Class;
    _class->name = name;
    // TODO
    _class->accessFlag = ACC_PUBLIC;
    _class->classLoader = this;
    _class->superClass = loadClass("java/lang/Object");
    _class->interfaces.push_back(loadClass("java/lang/Cloneable"));
    _class->interfaces.push_back(loadClass("java/lang/Serializable"));
    _class->initStarted = true;
    if(verboseFlag){
        cout << "[Loaded " << name << "]\n";
    }
    classMap[name] = _class;
    return _class;
}