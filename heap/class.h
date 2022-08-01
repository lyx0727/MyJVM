#ifndef CLASS_GUARD
#define CLASS_GUARD
#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include "../classfile/classfile.h"
#include "../rtda/thread.h"
#include "../rtda/slot.h"
#include "object.h"
#include "constant_pool.h"
#include "class_loader.h"
#include "class_member.h"

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

inline bool isSuper(uint16_t accessFlag){ return (accessFlag & ACC_SUPER) != 0; } 
inline bool isPublic(uint16_t accessFlag){ return (accessFlag & ACC_PUBLIC) != 0; } 
inline bool isProtected(uint16_t accessFlag){ return (accessFlag & ACC_PROTECTED) != 0; } 
inline bool isPrivate(uint16_t accessFlag){ return (accessFlag & ACC_PRIVATE) != 0; } 
inline bool isStatic(uint16_t accessFlag){ return (accessFlag & ACC_STATIC) != 0; } 
inline bool isFinal(uint16_t accessFlag){ return (accessFlag & ACC_FINAL) != 0; } 
inline bool isAbstract(uint16_t accessFlag){ return (accessFlag & ACC_ABSTRACT) != 0; } 
inline bool isInterface(uint16_t accessFlag){ return (accessFlag & ACC_INTERFACE) != 0; } 
inline bool isNative(uint16_t accessFlag){ return (accessFlag & ACC_NATIVE) != 0; } 

struct Field;
struct Method;
struct ClassLoader;
struct ConstantPool;
struct Object;

struct Class{
    uint16_t                 accessFlag;
    std::string              name;
    std::string              superClassName;
    std::vector<std::string> interfaceNames;
    ::ConstantPool*          constantPool;
    std::vector<Field*>      fields;
    std::vector<Method*>     methods;
    ClassLoader*             classLoader;
    Class*                   superClass;
    std::vector<Class*>      interfaces;
    uint32_t                 instanceSlotCount;             
    uint32_t                 staticSlotCount;             
    Slots*                   staticVars;     
    //  has <cinit> started
    bool                     initStarted; 

    Class(): constantPool(nullptr), classLoader(nullptr), superClass(nullptr), staticVars(nullptr){}
    Class(classfile::Classfile& cf);
    ~Class();

    Object* newObject();
    // array
    Object* newArray(unsigned int count);
    Object* newMultiDimensionalArray(int* counts, int dimensions);
    Class* getArrayClass() const;
    Class* getComponentClass() const;
    
    // type
    bool isArray() const { return name[0] == '['; }
    // attribute
    const std::string getPackageName() const;
    Method* getStaticMethod(const std::string& name, const std::string& descriptor) const;
    Method* getMainMethod() const { return getStaticMethod("main", "([Ljava/lang/String;)V"); }
    Method* getClintMethod() const { return getStaticMethod("<clinit>", "()V"); }
    std::vector<Field*> getFields(const std::vector<classfile::MemberInfo*>& cfFields);
    std::vector<Method*> getMethods(const std::vector<classfile::MemberInfo*>& cfMethods);
    Field* lookupField(const std::string& name, const std::string& descriptor) const;
    Method* lookupMethodInClass(const std::string& name, const std::string& descriptor) const;
    Method* lookupMethodInInterfaces(const std::string& name, const std::string& descriptor) const;
    Method* lookupMethod(const std::string& name, const std::string& descriptor) const;
    Method* lookupInterfaceMethod(const std::string& name, const std::string& descriptor) const;
    // relation with another class
    bool isSubClassOf(const Class* c) const;
    bool isSuperClassOf(const Class* c) const;
    bool isSubInterfaceOf(const Class* iface) const;
    bool isSuperInterfaceOf(const Class* iface) const;
    bool isImplements(const Class* iface) const;
    bool isAccessibleTo(const Class* other) const { return isPublic() || getPackageName() == other->getPackageName(); }      
    bool isAssignableFrom(const Class* other) const;
    // access 
    bool isSuper() const { return ::isSuper(accessFlag); } 
    bool isPublic() const { return ::isPublic(accessFlag); } 
    bool isAbstract() const { return ::isAbstract(accessFlag); }
    bool isInterface() const { return ::isInterface(accessFlag); }
    // init
    void startInit() { initStarted = true; }
    // load
    void resolveSuperClass();
    void resolveInterfaces();
    void verify();
    void prepare();
    void link();
    unsigned int calcFieldSlotIds(bool staticOrNot);
    void calcInstanceFieldSlotIds();
    void calcStaticFieldSlotIds();
    void allocAndInitStaticVars();
    void initStaticFinalVar(Field* field);
};

const std::map<std::string, std::string> primitiveTypeMap = {
    { "void",    "V" },
    { "boolean", "Z" },
    { "byte",    "B" },
    { "short",   "S" },
    { "int",     "I" },
    { "long",    "J" },
    { "char",    "C" },
    { "float",   "F" },
    { "double",  "D" },
};

const std::string toDescriptor(const std::string& className);
const std::string toClassName(const std::string& descriptor);

#endif