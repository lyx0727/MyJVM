#ifndef Classfile_GUARD
#define Classfile_GUARD

#include <vector>
#include <cstdint>
#include "../utils/utils.h"
#include "class_reader.h"
#include "constant_pool.h"
#include "member_info.h"
#include "attribute_info.h"

namespace classfile{
class Classfile{
private:
    uint32_t                    magic;
    uint16_t                    minorVersion;
    uint16_t                    majorVersion;
    ConstantPool                constantPool;
    uint16_t                    accessFlags;
    uint16_t                    thisClass;
    uint16_t                    superClass;
    std::vector<uint16_t>       interfaces;
    std::vector<MemberInfo*>    fields;
    std::vector<MemberInfo*>    methods;
    std::vector<AttributeInfo*> attributes;
public:
    Classfile(const std::vector<Byte>& classData){ 
        ClassReader cr(classData);
        read(cr); 
    }
    ~Classfile();

    void read(ClassReader& cr);
    void readAndCheckMagic(ClassReader& cr);
    void readAndCheckVersion(ClassReader& cr);

    const uint16_t getMinorVersion() const { return minorVersion; }
    const uint16_t getMajorVersion() const { return majorVersion; }
    ConstantPool& getConstantPool() { return constantPool; }
    const uint16_t getAccessFlags() const { return accessFlags; } 
    const std::vector<MemberInfo*>&  getFields() const { return fields; } 
    const std::vector<MemberInfo*>&  getMethods() const { return methods; } 
    const std::string getClassName() const { return constantPool.getClassName(thisClass); }
    const std::string getSuperClassName() const {
        if(superClass > 0){
            return constantPool.getClassName(superClass);
        }
        // 'java.lang.Object' has no super class
        return "";
    }
    const std::vector<std::string> getInterfacesNames() const;

    MemberInfo* getMainMethod();

    const std::string toString() const; 
};
}

#endif