#ifndef INFO_GUARD
#define INFO_GUARD
#include <cstdint>
#include <string>
#include "class_reader.h"
#include "constant_pool.h"

namespace classfile{
class ConstantPool;

namespace ConstantInfoType{
    enum ConstantInfoType {
        Class              = 7,   
        Fieldref           = 9,      
        Methodref          = 10,       
        InterfaceMethodref = 11,                
        String             = 8,    
        Integer            = 3,     
        Float              = 4,   
        Long               = 5,  
        Double             = 6,    
        NameAndType        = 12,         
        Utf8               = 1,  
        MethodHandle       = 15,          
        MethodType         = 16,        
        InvokeDynamic      = 18           
    };
}

struct ConstantInfo{
    virtual uint8_t getTag() = 0;
    virtual void readInfo(ClassReader& cr) = 0;
    virtual const std::string toString() const = 0;
    virtual ~ConstantInfo(){}
    ConstantInfo& operator=(const ConstantInfo& c) = delete;
};

struct ConstantIntegerInfo : public ConstantInfo {  
    uint8_t getTag(){ return ConstantInfoType::Integer; }
    int val; 
    void readInfo(ClassReader& cr){
        val = toInt(cr.readUint32());
    }
    const std::string toString() const { return std::to_string(val); }
};
struct ConstantFloatInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantInfoType::Float; }
    float val;
    void readInfo(ClassReader& cr){
        val = toFloat(cr.readUint32());
    }
    const std::string toString() const { return std::to_string(val); }
};
struct ConstantLongInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantInfoType::Long; }
    long val;
    void readInfo(ClassReader& cr){
        val = toLong(cr.readUint64());
    }
    const std::string toString() const { return std::to_string(val); }
};
struct ConstantDoubleInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantInfoType::Double; }
    double val;
    void readInfo(ClassReader& cr){
        val = toDouble(cr.readUint64());
    }
    const std::string toString() const { return std::to_string(val); }
};
struct ConstantUtf8Info : public ConstantInfo {
    uint8_t getTag(){ return ConstantInfoType::Utf8; }
    std::string str;
    static const std::string decodeMUTF8(const std::vector<Byte>& bytes); 
    void readInfo(ClassReader& cr){
        uint16_t length = cr.readUint16();
        std::vector<Byte> bytes = cr.readBytes(length);
        str = decodeMUTF8(bytes);
    }
    const std::string toString() const { return str; }
};
struct ConstantStringInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantInfoType::String; }
    ConstantPool& cp;
    uint16_t stringIndex;
    ConstantStringInfo(ConstantPool& cp): cp(cp){}
    void readInfo(ClassReader& cr){
        stringIndex = cr.readUint16();
    }
    const std::string getString() const;
    const std::string toString() const { return getString(); }
};
struct ConstantClassInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantInfoType::Class; }
    ConstantPool& cp;
    uint16_t nameIndex;
    ConstantClassInfo(ConstantPool& cp): cp(cp){}
    void readInfo(ClassReader& cr){
        nameIndex = cr.readUint16();
    }
    const std::string getName() const;
    const std::string toString() const { return getName(); }
};
struct ConstantNameAndTypeInfo : public ConstantInfo {
    uint8_t getTag(){ return ConstantInfoType::NameAndType; }
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    void readInfo(ClassReader& cr){
        nameIndex = cr.readUint16();
        descriptorIndex = cr.readUint16();
    }
    const std::string toString() const { return ""; }
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
    const std::string getClassName() const;
    const std::pair<std::string, std::string> getNameAndDescriptor() const;
    const std::string toString() const { 
        return getClassName() + ": " + getNameAndDescriptor().first + " " + getNameAndDescriptor().second;
    }
};
struct ConstantFieldrefInfo : public ConstantMemberrefInfo {
    ConstantFieldrefInfo(ConstantPool& cp): ConstantMemberrefInfo(cp, ConstantInfoType::Fieldref){}
};
struct ConstantMethodrefInfo : public ConstantMemberrefInfo {
    ConstantMethodrefInfo(ConstantPool& cp): ConstantMemberrefInfo(cp, ConstantInfoType::Methodref){}
};
struct ConstantInterfaceMethodrefInfo : public ConstantMemberrefInfo {
    ConstantInterfaceMethodrefInfo(ConstantPool& cp): ConstantMemberrefInfo(cp, ConstantInfoType::InterfaceMethodref){}
};
struct ConstantMethodTypeInfo : public ConstantInfo {
    uint16_t descriptorIndex;
    uint8_t getTag(){ return ConstantInfoType::MethodType; }
    void readInfo(ClassReader& cr){
        descriptorIndex = cr.readUint16();
    }
    const std::string toString() const { return ""; }
};
struct ConstantMethodHandleInfo : public ConstantInfo {
    uint8_t  referenceKind;
    uint16_t referenceIndex;
    uint8_t getTag(){ return ConstantInfoType::MethodHandle; }
    void readInfo(ClassReader& cr){
        referenceKind = cr.readUint8();
        referenceIndex = cr.readUint16();
    }
    const std::string toString() const { return ""; }
};
struct ConstantInvokeDynamicInfo : public ConstantInfo {
    uint16_t bootstrapMethodAttrIndex;
    uint16_t nameAndTypeIndex;
    uint8_t getTag(){ return ConstantInfoType::InvokeDynamic; }
    void readInfo(ClassReader& cr){
        bootstrapMethodAttrIndex = cr.readUint16();
        nameAndTypeIndex = cr.readUint16();
    }
    const std::string toString() const { return ""; }
};

ConstantInfo* readConstantInfo(ClassReader& cr, ConstantPool& cp);
ConstantInfo* getConstantInfo(uint8_t tag);
}
#endif