#ifndef CLASS_GUARD
#define CLASS_GUARD
#include <cstdint>
#include <string>
#include <vector>
#include "../classfile/classfile.h"
#include "../rtda/thread.h"
#include "class_member.h"
#include "constant_pool.h"
#include "class_loader.h"

// constexpr uint16_t ACC_PUBLIC       = 0x0001;  
// constexpr uint16_t ACC_PRIVATE      = 0x0002;
// constexpr uint16_t ACC_PROTECTED    = 0x0004; 
// constexpr uint16_t ACC_STATIC       = 0x0008;
// constexpr uint16_t ACC_FINAL        = 0x0010;
// constexpr uint16_t ACC_SUPER        = 0x0020;
// constexpr uint16_t ACC_SYNCHRONIZED = 0x0020;
// constexpr uint16_t ACC_VOLATILE     = 0x0040;
// constexpr uint16_t ACC_BRIDGE       = 0x0040;
// constexpr uint16_t ACC_TRANSIENT    = 0x0080;
// constexpr uint16_t ACC_VARARGS      = 0x0080;
// constexpr uint16_t ACC_NATIVE       = 0x0100;
// constexpr uint16_t ACC_INTERFACE    = 0x0200;
// constexpr uint16_t ACC_ABSTRACT     = 0x0400;
// constexpr uint16_t ACC_STRICT       = 0x0800;
// constexpr uint16_t ACC_SYNTHETIC    = 0x1000;
// constexpr uint16_t ACC_ANNOTATION   = 0x2000;
// constexpr uint16_t ACC_ENUM         = 0x4000;

struct Field;
struct Method;
class ClassLoader;

struct Class{
    uint16_t                 accessFlag;
    std::string              name;
    std::string              superClassName;
    std::vector<std::string> interfaceNames;
    ConstantPool             constantPool;
    std::vector<Field*>      fields;
    std::vector<Method*>     methods;
    ClassLoader*             classLoader;
    Class*                   superClass;
    std::vector<Class*>      interfaces;
    uint32_t                 instanceSlotCount;             
    uint32_t                 staticSlotCount;             
    Slots                    staticVars;      

    Class(classfile::Classfile& cf);

    // bool isPublic() const { return (accessFlag & ACC_PUBLIC) != 0; }       
};


#endif