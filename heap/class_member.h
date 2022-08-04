#ifndef CLASS_MEMBER_GUARD
#define CLASS_MEMBER_GUARD
#include "class.h"
#include "descriptor_parser.h"
#include "exception_table.h"

struct Class;
struct ExceptionTable;

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
    Field(Class* _class, classfile::MemberInfo* fieldInfo)
    : ClassMember(_class, fieldInfo), slotId(0U), constValueIndex(0U){
        classfile::ConstantValueAttribute* valAttr = fieldInfo->getConstantValueAttribute();
        if(valAttr != nullptr){
            constValueIndex = (unsigned int)valAttr->constantValueIndex;
        }
    } 
};

struct Method : public ClassMember{
    uint32_t                             maxStack;
    uint32_t                             maxLocals;
    std::vector<Byte>                    code;
    MethodDescriptor                     md;
    uint32_t                             argSlotCount;
    ExceptionTable*                      exceptionTable;
    classfile::LineNumberTableAttribute* lineNumberTable;
    Method(Class* _class, classfile::MemberInfo* methodInfo);
    ~Method();

    uint32_t calcArgSlotCount();
    void injectCodeAttribute();
    int findExceptionHandler(Class* exClass, int pc);
    int getLineNumber(int pc);
};

#endif