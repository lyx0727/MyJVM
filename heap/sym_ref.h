#ifndef SYM_REF_GUARD
#define SYM_REF_GUARD
#include <iostream>
#include "class.h"

struct SymRef{
    ConstantPool* cp;
    std::string   className;
    Class*       _class;
    SymRef(ConstantPool* cp, const std::string& className)
    : cp(cp), className(className), _class(nullptr){}

    Class* resolvedClass(){
        if(_class == nullptr){
            resolveClassRef();
        }
        return _class;
    }
    void resolveClassRef(){
        Class* d = cp->_class;
        Class* c = d->classLoader->loadClass(className);
        if(!c->isAccessibleTo(d)){
            std::cerr << "java.lang.IllegalAccessError" << std::endl;
            exit(1);
        }
        _class = c;
    }
};

struct ClassRef : public SymRef{
    ClassRef(ConstantPool* cp, classfile::ConstantClassInfo* classInfo)
        : SymRef(cp, classInfo->getName()) {}
};

struct MemberRef : public SymRef{
    std::string name;
    std::string descriptor;
    MemberRef(ConstantPool* cp, classfile::ConstantMemberrefInfo* memberInfo)
        : SymRef(cp, memberInfo->getClassName()){
        const std::pair<std::string, std::string>& p = memberInfo->getNameAndDescriptor();
        name = p.first;
        descriptor = p.second;
    }
};

struct FieldRef : public MemberRef{
    Field* field;
    FieldRef(ConstantPool* cp, classfile::ConstantFieldrefInfo* fieldInfo)
        : MemberRef(cp, fieldInfo){}

    Field* resolvedFeild(){
        if(field == nullptr){
            resolveFieldRef();
        }
        return field;
    }
    void resolveFieldRef(){
        Class* d = cp->_class;
        Class* c = resolvedClass();
        Field* field = lookupField(c, name, descriptor);
        if(field == nullptr){
            std::cerr << "java.lang.NoSuchFieldError" << std::endl;
            exit(1);
        }
        if(!field->isAccessibleTo(d)){
            std::cerr << "java.lang.IllegalAccessError" << std::endl;
            exit(1);
        }
        this->field = field;
    }
};

struct MethodRef : public MemberRef{
    Method* method;
    MethodRef(ConstantPool* cp, classfile::ConstantMethodrefInfo* methodRef)
        : MemberRef(cp, methodRef){}
};

struct InterfaceMethodRef : public MemberRef{
    Method* method;
    InterfaceMethodRef(ConstantPool* cp, classfile::ConstantInterfaceMethodrefInfo* interfaceMethodRef)
        : MemberRef(cp, interfaceMethodRef){
    }
};

#endif