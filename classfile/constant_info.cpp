#include "constant_info.h"
#include <iostream>
using namespace std;

namespace classfile{
const string ConstantUtf8Info::decodeMUTF8(const vector<Byte>& bytes){
    // TODO
    // decode Modified UTF8
    // not exactly same as decoding UTF8, but decode as UTF8 instead
    string res;
    for(Byte byte : bytes){
        res += byte;
    }
    return res;
}

ConstantInfo* getConstantInfo(uint8_t tag, ConstantPool& cp){
    switch(tag){
        case ConstantInfoType::Integer: return new ConstantIntegerInfo;
        case ConstantInfoType::Float: return new ConstantFloatInfo;
        case ConstantInfoType::Long: return new ConstantLongInfo;
        case ConstantInfoType::Double: return new ConstantDoubleInfo;
        case ConstantInfoType::Utf8: return new ConstantUtf8Info;
        case ConstantInfoType::String: return new ConstantStringInfo(cp);
        case ConstantInfoType::Class: return new ConstantClassInfo(cp);
        case ConstantInfoType::Fieldref: return new ConstantFieldrefInfo(cp);
        case ConstantInfoType::Methodref: return new ConstantMethodrefInfo(cp);
        case ConstantInfoType::InterfaceMethodref: return new ConstantInterfaceMethodrefInfo(cp);
        case ConstantInfoType::NameAndType: return new ConstantNameAndTypeInfo;
        case ConstantInfoType::MethodType: return new ConstantMethodTypeInfo;
        case ConstantInfoType::MethodHandle: return new ConstantMethodHandleInfo;
        case ConstantInfoType::InvokeDynamic: return new ConstantInvokeDynamicInfo;
        default:
            throw JavaLangClassFormatError(to_string(tag), __FILE__, __LINE__);
    }
}

ConstantInfo* readConstantInfo(ClassReader& cr, ConstantPool& cp){
    uint8_t tag = cr.readUint8();
    ConstantInfo* c = getConstantInfo(tag, cp);
    c->readInfo(cr);
    return c;
}

const string ConstantStringInfo::getString() const {
    return cp.getUtf8(stringIndex);
}

const string ConstantClassInfo::getName() const {
    return cp.getUtf8(nameIndex);
}

const std::string ConstantMemberrefInfo::getClassName() const {
    return cp.getClassName(classIndex);
}

const std::pair<std::string, std::string> ConstantMemberrefInfo::getNameAndDescriptor() const{
    return cp.getNameAndType(nameAndTypeIndex);
}
}