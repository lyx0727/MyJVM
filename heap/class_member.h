#ifndef CLASS_MEMBER_GUARD
#define CLASS_MEMBER_GUARD
#include "class.h"

struct ClassMember{
    uint16_t    accessFlag;
    std::string name;
    std::string descriptor;
    Class*      _class;

    ClassMember(Class* _class, MemberInfo* memberInfo): _class(_class){
        accessFlag = memberInfo->getAccessFlags();
        name = memberInfo->getName();
        descriptor = memberInfo->getDescriptor();
    }
};

struct Field : public ClassMember{ Field(Class* _class, MemberInfo* memberInfo): ClassMember(_class, memberInfo){} };
struct Method : public ClassMember{
    uint32_t          maxStack;
    uint32_t          maxLocals;
    std::vector<Byte> code;
    Method(Class* _class, MemberInfo* memberInfo): ClassMember(_class, memberInfo){}
};

std::vector<Field*> getFields(Class* _class, const std::vector<MemberInfo*>& cfFields);
std::vector<Method*> getFethods(Class* _class, const std::vector<MemberInfo*>& cfMethods);

#endif