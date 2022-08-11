#ifndef SYM_REF_GUARD
#define SYM_REF_GUARD
#include <iostream>
#include "class.h"

struct Class;
struct Field;
struct Method;
struct ConstantPool;

struct SymRef {
private:
    Class* _class;
public:
    ::ConstantPool* cp;
    std::string   className;
    SymRef(::ConstantPool* cp, const std::string& className)
    : _class(nullptr), cp(cp), className(className){}
    virtual ~SymRef(){}
    Class* resolvedClass();
    void resolveClassRef();
};

struct ClassRef : public SymRef{
    ClassRef(::ConstantPool* cp, classfile::ConstantClassInfo* classInfo)
        : SymRef(cp, classInfo->getName()) {}
};

struct MemberRef : public SymRef{
    std::string name;
    std::string descriptor;
    MemberRef(::ConstantPool* cp, classfile::ConstantMemberrefInfo* memberInfo)
        : SymRef(cp, memberInfo->getClassName()){
        std::pair<std::string, std::string> p = memberInfo->getNameAndDescriptor();
        name = p.first;
        descriptor = p.second;
    }
};

struct FieldRef : public MemberRef{
private:
    Field* field;
public:
    FieldRef(::ConstantPool* cp, classfile::ConstantFieldrefInfo* fieldInfo)
        : MemberRef(cp, fieldInfo), field(nullptr){}

    Field* resolvedField();
    void resolveFieldRef();
};

struct MethodRef : public MemberRef{
private:
    Method* method;
public:
    MethodRef(::ConstantPool* cp, classfile::ConstantMethodrefInfo* methodRef)
        : MemberRef(cp, methodRef), method(nullptr){}
    Method* resolvedMethod();
    void resolveMethodRef();
};

struct InterfaceMethodRef : public MemberRef{
private:
    Method* method;
public:
    InterfaceMethodRef(::ConstantPool* cp, classfile::ConstantInterfaceMethodrefInfo* interfaceMethodRef)
        : MemberRef(cp, interfaceMethodRef){
    }
    Method* resolvedInterfaceMethod();
    void resolveInterfaceMethodRef();
};

#endif