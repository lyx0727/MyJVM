#include <iostream>
#include <exception>
#include "class_loader.h"
using namespace std;

Class* ClassLoader::loadClass(const string& name){
    if(classMap.count(name)){
        return classMap.at(name);
    }
    // TODO
    return loadNonArrayClass(name);
}

pair<vector<Byte>, Entry*> ClassLoader::readClass(const std::string& name){
    pair<vector<Byte>, Entry*> p = cp->readClass(name);
    if(p.first.size() == 0){
        cerr << "java.lang.ClassNotFoundException: " << name << endl;
        exit(1);
    }
    return p;
}

Class* ClassLoader::parseClass(const std::vector<Byte>& data){
    try{
        classfile::Classfile cf(data);
        return new Class(cf);
    }
    catch(const exception& e){
        cerr << "class_loader.cpp: java.lang.ClassFormatError: ";
        cerr << e.what() << endl;
        exit(1);
    }
}

Class* ClassLoader::defineClass(const std::vector<Byte>& data){
    Class* _class = parseClass(data);
    _class->classLoader = this;
    resolveSuperClass(_class);
    resolveInterfaces(_class);
    classMap[_class->name] = _class;
    return _class;
}

Class* ClassLoader::loadNonArrayClass(const string& name){
    pair<vector<Byte>, Entry*> p = readClass(name);
    vector<Byte>& data = p.first;
    Entry* entry = p.second;
    Class* _class = defineClass(data);
    link(_class);
    cout << "[Loaded ]" << name << " from " << entry->toString() << "]\n";
    return _class;
}

void resolveSuperClass(Class* _class){
    if(_class->name == "java/lang/Object"){
        _class->superClass = nullptr;
        return;
    }
    _class->superClass = _class->classLoader->loadClass(_class->superClassName);
}

void resolveInterfaces(Class* _class){
    size_t interfaceCount = _class->interfaceNames.size();
    _class->interfaces = vector<Class*>(interfaceCount);
    for(size_t i = 0; i < interfaceCount; i++){
        _class->interfaces[i] = _class->classLoader->loadClass(_class->interfaceNames[i]);
    }
}

void verify(Class* _class){
    // TODO
}

void prepare(Class* _class){
    calcInstanceFieldSlotIds(_class);
    calcStaticFieldSlotIds(_class);
    allocAndInitStaticVars(_class);
}

void link(Class* _class){
    verify(_class);
    prepare(_class);
}

unsigned int calcFieldSlotIds(Class* _class, bool staticOrNot){
    unsigned int slotId = 0;
    if(!staticOrNot && _class->superClass != nullptr){
        slotId = _class->superClass->instanceSlotCount;
    }
    for(Field* field : _class->fields){
        bool flag = (field->isStatic() == staticOrNot);
        if(flag){
            field->slotId = slotId;
            slotId++;
            if(field->isLongOrDouble()){
                slotId++;
            }
        }
    }
    return slotId;
}
void calcInstanceFieldSlotIds(Class* _class){ _class->instanceSlotCount = calcFieldSlotIds(_class, false); }
void calcStaticFieldSlotIds(Class* _class){ _class->staticSlotCount = calcFieldSlotIds(_class, true); }

void allocAndInitStaticVars(Class* _class){
    _class->staticVars = Slots(_class->staticSlotCount);
    for(Field* field : _class->fields){
        if(field->isStatic() && field->isFinal()){
            initStaticFinalVar(_class, field);
        }
    }
}

void initStaticFinalVar(Class* _class, Field* field){
    Slots& vars = _class->staticVars;
    ConstantPool* cp = _class->constantPool;
    
    unsigned int cpIndex = field->constValueIndex;
    unsigned int slotId = field->slotId;
    if(cpIndex > 0){
        string& d = field->descriptor;
        if(d == "Z" || d == "B" || d == "C" || d == "S" || d == "I"){
            int val = cp->getConstant(cpIndex).getVal<int>();
            vars.set(slotId, val);
        }
        else if(d == "J"){
            long val = cp->getConstant(cpIndex).getVal<long>();
            vars.set(slotId, val);
        }
        else if(d == "F"){
            float val = cp->getConstant(cpIndex).getVal<float>();
            vars.set(slotId, val);
        }
        else if(d == "D"){
            double val = cp->getConstant(cpIndex).getVal<double>();
            vars.set(slotId, val);
        }
        else if(d == "Ljava/lang/String;"){
            // TODO
            cerr << "String TO DO" << endl;
        }
    }
}
