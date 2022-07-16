#ifndef CONSTANT_INFO_GUARD
#define CONSTANT_INFO_GUARD
#include <cstdint>
#include <string>
#include "class_reader.h"
#include "constant_pool.h"

class ConstantPool;

enum ConstantType {
    CONSTANT_Class              = 7,   
    CONSTANT_Fieldref           = 9,      
    CONSTANT_Methodref          = 10,       
    CONSTANT_InterfaceMethodref = 11,                
    CONSTANT_String             = 8,    
    CONSTANT_Integer            = 3,     
    CONSTANT_Float              = 4,   
    CONSTANT_Long               = 5,  
    CONSTANT_Double             = 6,    
    CONSTANT_NameAndType        = 12,         
    CONSTANT_Utf8               = 1,  
    CONSTANT_MethodHandle       = 15,          
    CONSTANT_MethodType         = 16,        
    CONSTANT_InvokeDynamic      = 18           
};

struct ConstantInfo{
    virtual uint8_t getTag() = 0;
    virtual void readInfo(ClassReader& cr) = 0;
    virtual ~ConstantInfo(){}
    ConstantInfo& operator=(const ConstantInfo& c) = delete;
};

struct ConstantIntegerInfo : public ConstantInfo {  
    uint8_t getTag(){ return ConstantType::CONSTANT_Integer; }
    int32_t val; 
    void readInfo(ClassReader& cr){
        val = (int32_t)cr.readUint32();
    }
    ~ConstantIntegerInfo(){}
};
struct ConstantFloatInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_Float; }
    float val;
    void readInfo(ClassReader& cr){
        val = (float)cr.readUint32();
    }
    ~ConstantFloatInfo(){}
};
struct ConstantLongInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_Long; }
    long val;
    void readInfo(ClassReader& cr){
        val = (long)cr.readUint64();
    }
    ~ConstantLongInfo(){}
};
struct ConstantDoubleInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_Double; }
    double val;
    void readInfo(ClassReader& cr){
        val = (double)cr.readUint64();
    }
    ~ConstantDoubleInfo(){}
};
struct ConstantUtf8Info : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_Utf8; }
    std::string str;
    static const std::string decodeMUTF8(const std::vector<Byte>& bytes); 
    void readInfo(ClassReader& cr){
        uint32_t length = (uint16_t)cr.readUint16();
        std::vector<Byte> bytes = cr.readBytes(length);
        str = decodeMUTF8(bytes);
    }
    ~ConstantUtf8Info(){}
};
struct ConstantStringInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_String; }
    ConstantPool& cp;
    uint16_t stringIndex;
    ConstantStringInfo(ConstantPool& cp): cp(cp){}
    void readInfo(ClassReader& cr){
        stringIndex = cr.readUint16();
    }
    // const std::string getString(){
    //     return cp.getUtf8(stringIndex);
    // }
    ~ConstantStringInfo(){}
};
struct ConstantClassInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_Class; }
    ConstantPool& cp;
    uint16_t nameIndex;
    ConstantClassInfo(ConstantPool& cp): cp(cp){}
    void readInfo(ClassReader& cr){
        nameIndex = cr.readUint16();
    }
    // const std::string getName(){
    //     return cp.getUtf8(nameIndex);
    // }
    ~ConstantClassInfo(){}
};
struct ConstantNameAndTypeInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_NameAndType; }
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    void readInfo(ClassReader& cr){
        nameIndex = cr.readUint16();
        descriptorIndex = cr.readUint16();
    }
    ~ConstantNameAndTypeInfo(){}
};
struct ConstantMemberrefInfo : public ConstantInfo{
    ConstantPool& cp;
    uint8_t tag;
    uint8_t getTag(){ return tag; }
    uint16_t classIndex;
    uint16_t nameAndTypeIndex;
    ConstantMemberrefInfo(ConstantPool& cp, uint8_t tag): cp(cp), tag(tag){}
    void readInfo(ClassReader& cr){
        classIndex = cr.readUint16();
        nameAndTypeIndex = cr.readUint16();
    }
    ~ConstantMemberrefInfo(){}
    // const std::string getClassName(){
    //     return cp.getClassName(classIndex);
    // }
    // const std::pair<std::string, std::string> getNameAndDescriptor(){
    //     return cp.getNameAndType(nameAndTypeIndex);
    // }
};
struct ConstantFieldrefInfo : public ConstantMemberrefInfo {
    ConstantFieldrefInfo(ConstantPool& cp): ConstantMemberrefInfo(cp, ConstantType::CONSTANT_Fieldref){}
};
struct ConstantMethodrefInfo : public ConstantMemberrefInfo {
    ConstantMethodrefInfo(ConstantPool& cp): ConstantMemberrefInfo(cp, ConstantType::CONSTANT_Methodref){}
};
struct ConstantInterfaceMethodrefInfo : public ConstantMemberrefInfo {
    ConstantInterfaceMethodrefInfo(ConstantPool& cp): ConstantMemberrefInfo(cp, ConstantType::CONSTANT_InterfaceMethodref){}
};
// TODO
struct ConstantMethodTypeInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_MethodType; }
    void readInfo(ClassReader& cr){}
    ~ConstantMethodTypeInfo(){}
};
// TODO
struct ConstantMethodHandleInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_MethodHandle; }
    void readInfo(ClassReader& cr){}
    ~ConstantMethodHandleInfo(){}
};
// TODO
struct ConstantInvokeDynamicInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantType::CONSTANT_InvokeDynamic; }
    void readInfo(ClassReader& cr){}
    ~ConstantInvokeDynamicInfo(){}
};

ConstantInfo* readConstantInfo(ClassReader& cr, ConstantPool& cp);
ConstantInfo* getConstantInfo(uint8_t tag);

#endif