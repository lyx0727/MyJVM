#ifndef CLASS_MEMBER_GUARD
#define CLASS_MEMBER_GUARD
#include "class.h"

class Class;

constexpr uint16_t ACC_PUBLIC       = 0x0001;  
constexpr uint16_t ACC_PRIVATE      = 0x0002;
constexpr uint16_t ACC_PROTECTED    = 0x0004; 
constexpr uint16_t ACC_STATIC       = 0x0008;
constexpr uint16_t ACC_FINAL        = 0x0010;
constexpr uint16_t ACC_SUPER        = 0x0020;
constexpr uint16_t ACC_SYNCHRONIZED = 0x0020;
constexpr uint16_t ACC_VOLATILE     = 0x0040;
constexpr uint16_t ACC_BRIDGE       = 0x0040;
constexpr uint16_t ACC_TRANSIENT    = 0x0080;
constexpr uint16_t ACC_VARARGS      = 0x0080;
constexpr uint16_t ACC_NATIVE       = 0x0100;
constexpr uint16_t ACC_INTERFACE    = 0x0200;
constexpr uint16_t ACC_ABSTRACT     = 0x0400;
constexpr uint16_t ACC_STRICT       = 0x0800;
constexpr uint16_t ACC_SYNTHETIC    = 0x1000;
constexpr uint16_t ACC_ANNOTATION   = 0x2000;
constexpr uint16_t ACC_ENUM         = 0x4000;

inline bool isPublic(uint16_t accessFlag){ return (accessFlag & ACC_PUBLIC) != 0; } 
inline bool isStatic(uint16_t accessFlag){ return (accessFlag & ACC_STATIC) != 0; } 
inline bool isFinal(uint16_t accessFlag){ return (accessFlag & ACC_FINAL) != 0; } 

struct ClassMember{
    uint16_t    accessFlag;
    std::string name;
    std::string descriptor;
    Class*      _class;
    bool isStatic() const { return ::isStatic(accessFlag); }
    bool isFinal() const { return ::isFinal(accessFlag); }
    bool isLongOrDouble() const { return descriptor == "J" || descriptor == "D"; }
    ClassMember(Class* _class, classfile::MemberInfo* memberInfo): _class(_class){
        accessFlag = memberInfo->getAccessFlags();
        name = memberInfo->getName();
        descriptor = memberInfo->getDescriptor();
    }
};

struct Field : public ClassMember{ 
    unsigned int slotId;
    unsigned int constValueIndex;
    Field(Class* _class, classfile::MemberInfo* memberInfo): ClassMember(_class, memberInfo){
        classfile::ConstantValueAttribute* valAttr = memberInfo->getConstantValueAttribute();
        if(valAttr != nullptr){
            constValueIndex = (unsigned int)valAttr->constantValueIndex;
        }
    } 
};
struct Method : public ClassMember{
    uint32_t          maxStack;
    uint32_t          maxLocals;
    std::vector<Byte> code;
    Method(Class* _class, classfile::MemberInfo* memberInfo): ClassMember(_class, memberInfo){}
};

std::vector<Field*> getFields(Class* _class, const std::vector<classfile::MemberInfo*>& cfFields);
std::vector<Method*> getMethods(Class* _class, const std::vector<classfile::MemberInfo*>& cfMethods);

#endif