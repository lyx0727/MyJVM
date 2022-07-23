#ifndef HEAP_CONSTANT_POOL_GUARD
#define HEAP_CONSTANT_POOL_GUARD
#include "class.h"

union Constant{
    int intVal;
    long longVal;
    float floatVal;
    double doubleVal;
    void* ref;
};

class ConstantPool{
private:
    Class*                _class;
    std::vector<Constant> consts;
public:
    ConstantPool(Class* _class, classfile::ConstantPool& cp);
    Constant getConstant(unsigned int index) const { return consts[index]; }
};

struct SymRef{
    ConstantPool& cp;
    std::string   className;
    Class*       _class;
    SymRef(ConstantPool& cp, const std::string& className)
    : cp(cp), className(className){}
};

struct ClassRef : public SymRef{
    ClassRef(ConstantPool& cp, classfile::ConstantClassInfo* classInfo)
        : SymRef(cp, classInfo->getName()) {}
};

struct MemberRef : public SymRef{
    std::string name;
    std::string descriptor;
    MemberRef(ConstantPool& cp, classfile::ConstantMemberrefInfo* memberInfo)
        : SymRef(cp, memberInfo->getClassName()){
        const std::pair<std::string, std::string>& p = memberInfo->getNameAndDescriptor();
        name = p.first;
        descriptor = p.second;
    }
};

struct FieldRef : public SymRef{
    Field* field;
    FieldRef(ConstantPool& cp, classfile::ConstantFieldrefInfo* fieldInfo)
        : SymRef(cp, fieldInfo->getClassName()){

    }
};

struct MethodRef : public SymRef{
    Method* method;
    MethodRef(ConstantPool& cp, classfile::ConstantMethodrefInfo* methodRef)
        : SymRef(cp, methodRef->getClassName()){
    }
};

struct InterfaceMethodRef : public SymRef{
    Method* method;
    InterfaceMethodRef(ConstantPool& cp, classfile::ConstantInterfaceMethodrefInfo* interfaceMethodRef)
        : SymRef(cp, interfaceMethodRef->getClassName()){
    }
};

#endif