#include <iostream>
#include <exception>
#include "class_loader.h"
using namespace std;

ClassLoader::~ClassLoader(){
    for(auto it = classMap.begin(); it != classMap.end(); ){
        delete it->second;
        it->second = nullptr;
        it = classMap.erase(it);
    }
    // for(auto it = internedStrings.begin(); it != internedStrings.end();){
    //     delete it->second;
    //     it->second = nullptr;
    //     it = internedStrings.erase(it);
    // }
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
    _class->interfaces.push_back(loadClass("java/io/Serializable"));
    _class->initStarted = true;
    if(verboseFlag){
        cout << "[Loaded " << name << "]\n";
    }
    classMap[name] = _class;
    return _class;
}

Class* ClassLoader::getPrimitiveArrayClass(uint8_t atype){
    switch(atype){
        case AT_BOOLEAN: return loadClass("[Z");
        case AT_BYTE:    return loadClass("[B");
        case AT_CHAR:    return loadClass("[C");
        case AT_SHORT:   return loadClass("[S");
        case AT_INT:     return loadClass("[I");
        case AT_LONG:    return loadClass("[J");
        case AT_FLOAT:   return loadClass("[F");
        case AT_DOUBLE:  return loadClass("[D");
    }
    cerr << "Invalid atype: " << to_string(atype) << endl;
    exit(1);
    return nullptr;  
}

Object* ClassLoader::JString(const string& str){
    if(internedStrings.count(str)){
        return internedStrings.at(str);
    }
    u16string u16str = utf8_to_utf16(str);
    unsigned int len = u16str.length();
    Object* jChars = loadClass("[C")->newArray(len);
    char16_t* chars = jChars->getChars();
    for(unsigned int i = 0; i < len; i++){
        chars[i] = u16str[i];
    }
    // 'String' has a field: char[] value
    Object* jStr = loadClass("java/lang/String")->newObject();
    jStr->setRefVar("value", "[C", jChars);
    internedStrings[str] = jStr;
    return jStr;
}

Object* ClassLoader::createArgsArray(const vector<string>& args){
    unsigned int len = args.size();
    Object* argsArr = loadClass("java/lang/String")->getArrayClass()->newArray(len);
    Object** refs = argsArr->getRefs();
    for(unsigned int i = 0; i < len; i++){
        refs[i] = JString(args[i]);
    }
    return argsArr;
}
