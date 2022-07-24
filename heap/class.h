#ifndef CLASS_GUARD
#define CLASS_GUARD
#include <cstdint>
#include <string>
#include <vector>
#include "../classfile/classfile.h"
#include "../rtda/thread.h"
#include "constant_pool.h"
#include "class_loader.h"

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
inline bool isProtected(uint16_t accessFlag){ return (accessFlag & ACC_PROTECTED) != 0; } 
inline bool isPrivate(uint16_t accessFlag){ return (accessFlag & ACC_PRIVATE) != 0; } 
inline bool isStatic(uint16_t accessFlag){ return (accessFlag & ACC_STATIC) != 0; } 
inline bool isFinal(uint16_t accessFlag){ return (accessFlag & ACC_FINAL) != 0; } 
inline bool isAbstract(uint16_t accessFlag){ return (accessFlag & ACC_ABSTRACT) != 0; } 
inline bool isInterface(uint16_t accessFlag){ return (accessFlag & ACC_INTERFACE) != 0; } 

struct Field;
struct Method;
class ClassLoader;

struct Class{
    uint16_t                 accessFlag;
    std::string              name;
    std::string              superClassName;
    std::vector<std::string> interfaceNames;
    ConstantPool*            constantPool;
    std::vector<Field*>      fields;
    std::vector<Method*>     methods;
    ClassLoader*             classLoader;
    Class*                   superClass;
    std::vector<Class*>      interfaces;
    uint32_t                 instanceSlotCount;             
    uint32_t                 staticSlotCount;             
    Slots                    staticVars;      

    Class(classfile::Classfile& cf);
    ~Class(){ delete constantPool; }

    const std::string getPackageName() const;

    bool isSubClassOf(Class* c) const;
    bool isSubInterfaceOf(Class* iface) const;
    bool isImplements(Class* iface) const;

    bool isPublic() const { return ::isPublic(accessFlag); } 
    bool isAbstract() const { return ::isAbstract(accessFlag); }
    bool isInterface() const { return ::isInterface(accessFlag); }
    bool isAccessibleTo(Class* other) const { return isPublic() || getPackageName() == other->getPackageName(); }      
    bool isAssignableFrom(Class* other) const {
        if(this == other) return true;
        if(other->isInterface()){
            return isImplements(other);
        }
        else{
            return isSubClassOf(other);
        }
    }
};

struct ClassMember{
    uint16_t    accessFlag;
    std::string name;
    std::string descriptor;
    Class*      _class;
    bool isStatic() const { return ::isStatic(accessFlag); }
    bool isFinal() const { return ::isFinal(accessFlag); }
    bool isPublic() const { return ::isPublic(accessFlag); }
    bool isProtected() const { return ::isProtected(accessFlag); }
    bool isPrivate() const { return ::isPrivate(accessFlag); }
    bool isLongOrDouble() const { return descriptor == "J" || descriptor == "D"; }
    ClassMember(Class* _class, classfile::MemberInfo* memberInfo): _class(_class){
        accessFlag = memberInfo->getAccessFlags();
        name = memberInfo->getName();
        descriptor = memberInfo->getDescriptor();
    }

    bool isAccessibleTo(Class* d){
        if(isPublic()) return true;
        Class* c = _class;
        if(isProtected()){
            return d == c || d->isSubClassOf(c) || c->getPackageName() == d->getPackageName();
        }
        if(!isPrivate()){
            return c->getPackageName() == d->getPackageName();
        }
        return d == c;
    }
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
};
struct Method : public ClassMember{
    uint32_t          maxStack;
    uint32_t          maxLocals;
    std::vector<Byte> code;
    Method(Class* _class, classfile::MemberInfo* methodInfo): ClassMember(_class, methodInfo){
        classfile::CodeAttribute* codeAttr = methodInfo->getCodeAttribute();
        code = codeAttr->code;
        maxLocals = codeAttr->maxLocals;
        maxStack = codeAttr->maxStack;
    }
};

std::vector<Field*> getFields(Class* _class, const std::vector<classfile::MemberInfo*>& cfFields);
std::vector<Method*> getMethods(Class* _class, const std::vector<classfile::MemberInfo*>& cfMethods);

Field* lookupField(Class* _class, const std::string& name, const std::string& descriptor);

#endif