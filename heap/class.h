#ifndef CLASS_GUARD
#define CLASS_GUARD
#include <cstdint>
#include <string>
#include <vector>
#include "../classfile/classfile.h"
#include "../rtda/thread.h"

constexpr int ACC_PUBLIC       = 0x0001;  
constexpr int ACC_PRIVATE      = 0x0002;
constexpr int ACC_PROTECTED    = 0x0004; 
constexpr int ACC_STATIC       = 0x0008;
constexpr int ACC_FINAL        = 0x0010;
constexpr int ACC_SUPER        = 0x0020;
constexpr int ACC_SYNCHRONIZED = 0x0020;
constexpr int ACC_VOLATILE     = 0x0040;
constexpr int ACC_BRIDGE       = 0x0040;
constexpr int ACC_TRANSIENT    = 0x0080;
constexpr int ACC_VARARGS      = 0x0080;
constexpr int ACC_NATIVE       = 0x0100;
constexpr int ACC_INTERFACE    = 0x0200;
constexpr int ACC_ABSTRACT     = 0x0400;
constexpr int ACC_STRICT       = 0x0800;
constexpr int ACC_SYNTHETIC    = 0x1000;
constexpr int ACC_ANNOTATION   = 0x2000;
constexpr int ACC_ENUM         = 0x4000;

class Class{
private:
    uint16_t                 accessFlag;
    std::string              name;
    std::string              superClassName;
    std::vector<std::string> interfaceNames;
    ConstantPool&            constantPool;
    // std::vector<Feild*>      fields;
    // std::vector<Method*>     method;
    // ClassLoader                 classLoader;
    Class*                   superClass;
    std::vector<Class*>      interfaces;
    uint32_t                 instanceSlotCount;             
    uint32_t                 staticSlotCount;             
    std::vector<Slot>       staticVars;      

public:
    Class(Classfile& cf): constantPool(cf.getConstantPool()){
        accessFlag = cf.getAccessFlags();
        name = cf.getClassName();
        superClassName = cf.getSuperClassName();
        interfaceNames = cf.getInterfacesNames();
        
        // TODO
    }

    bool isPublic(Class* _class){ return (accessFlag & ACC_PUBLIC) != 0; }       
};


#endif