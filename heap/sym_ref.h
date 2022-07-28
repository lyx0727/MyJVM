#ifndef SYM_REF_GUARD
#define SYM_REF_GUARD
#include <iostream>
#include "class.h"

struct SymRef{
private:
    Class* _class;
public:
    ConstantPool* cp;
    std::string   className;
    SymRef(ConstantPool* cp, const std::string& className)
    : _class(nullptr), cp(cp), className(className){}

    Class* resolvedClass() {
        if(_class == nullptr){
            try{
                resolveClassRef();
            }catch(...){
                throw;
            }
        }
        return _class;
    }
    void resolveClassRef() {
        Class* d = cp->_class;
        Class* c = d->classLoader->loadClass(className);
        if(!c->isAccessibleTo(d)){
            throw JavaLangIllegalAccessError(className, __FILE__, __LINE__);
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
private:
    Field* field;
public:
    FieldRef(ConstantPool* cp, classfile::ConstantFieldrefInfo* fieldInfo)
        : MemberRef(cp, fieldInfo), field(nullptr){}

    Field* resolvedField(){
        if(field == nullptr){
            try{
                resolveFieldRef();
            }catch(...){
                throw;
            }
        }
        return field;
    }
    void resolveFieldRef() {
        Class* d = cp->_class;
        Class* c = resolvedClass();
        field = c->lookupField(name, descriptor);
        if(field == nullptr){
            throw JavaLangClassNoSuchFieldError(name, __FILE__, __LINE__);
        }
        if(!field->isAccessibleTo(d)){
            throw JavaLangIllegalAccessError(name, __FILE__, __LINE__);
        }
    }
};

struct MethodRef : public MemberRef{
private:
    Method* method;
public:
    MethodRef(ConstantPool* cp, classfile::ConstantMethodrefInfo* methodRef)
        : MemberRef(cp, methodRef), method(nullptr){}
    Method* resolvedMethod(){
        if(method == nullptr){
            try{
                resolveMethodRef();
            }catch(...){
                throw;
            }
        }
        return method;
    }
    void resolveMethodRef() {
        Class* d = cp->_class;
        Class* c = resolvedClass();
        if(c->isInterface()){
            throw JavaLangIncompatibleClassChangeError(c->name + " is interface", __FILE__, __LINE__);
        }
        method = c->lookupMethod(name, descriptor);
        if(method == nullptr){
            throw JavaLangNoSuchMethodError(name, __FILE__, __LINE__);
        }
        if(!method->isAccessibleTo(d)){
            throw JavaLangIllegalAccessError(name, __FILE__, __LINE__);
        }
    }
};

struct InterfaceMethodRef : public MemberRef{
private:
    Method* method;
public:
    InterfaceMethodRef(ConstantPool* cp, classfile::ConstantInterfaceMethodrefInfo* interfaceMethodRef)
        : MemberRef(cp, interfaceMethodRef){
    }
    Method* resolvedInterfaceMethod(){
        if(method == nullptr){
            resolveInterfaceMethodRef();
        }
        return method;
    }
    void resolveInterfaceMethodRef(){
        Class* d = cp->_class;
        Class* c = resolvedClass();
        if(!c->isInterface()){
            throw JavaLangIncompatibleClassChangeError(c->name + " is not interface", __FILE__, __LINE__);
        }
        method = c->lookupInterfaceMethod(name, descriptor);
        if(method == nullptr){
            throw JavaLangNoSuchMethodError(name, __FILE__, __LINE__);
        }
        if(!method->isAccessibleTo(d)){
            throw JavaLangIllegalAccessError(name, __FILE__, __LINE__);
        }
    }
};

#endif