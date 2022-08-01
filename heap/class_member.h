#ifndef CLASS_MEMBER_GUARD
#define CLASS_MEMBER_GUARD
#include "class.h"
#include "descriptor_parser.h"

class Class;

struct ClassMember{
    uint16_t    accessFlag;
    std::string name;
    std::string descriptor;
    Class*      _class;
    bool isStatic() const;
    bool isFinal() const;
    bool isAbstract() const;
    bool isNative() const;

    bool isPublic() const;
    bool isProtected() const;
    bool isPrivate() const;
    bool isLongOrDouble() const;
    ClassMember(Class* _class, classfile::MemberInfo* memberInfo): _class(_class){
        accessFlag = memberInfo->getAccessFlags();
        name = memberInfo->getName();
        descriptor = memberInfo->getDescriptor();
    }

    bool isAccessibleTo(Class* d) const;
};

struct Field : public ClassMember{ 
    unsigned int slotId;
    unsigned int constValueIndex;
    Field(Class* _class, classfile::MemberInfo* fieldInfo): ClassMember(_class, fieldInfo){
        classfile::ConstantValueAttribute* valAttr = fieldInfo->getConstantValueAttribute();
        if(valAttr != nullptr){
            constValueIndex = (unsigned int)valAttr->constantValueIndex;
        }
    } 
    ~Field(){ std::cout << "\t\t\t~" << name << std::endl; }
};

struct Method : public ClassMember{
    uint32_t          maxStack;
    uint32_t          maxLocals;
    std::vector<Byte> code;
    uint32_t argSlotCount;
    Method(Class* _class, classfile::MemberInfo* methodInfo)
        : ClassMember(_class, methodInfo){
        classfile::CodeAttribute* codeAttr = methodInfo->getCodeAttribute();
        if(codeAttr != nullptr){
            code = codeAttr->code;
            maxLocals = codeAttr->maxLocals;
            maxStack = codeAttr->maxStack;
        }
        calcArgSlotCount();
    }
    uint32_t calcArgSlotCount();
    
};

#endif