#include "constant_info.h"
#include <iostream>
using namespace std;

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
        case ConstantType::Integer: return new ConstantIntegerInfo;
        case ConstantType::Float: return new ConstantFloatInfo;
        case ConstantType::Long: return new ConstantLongInfo;
        case ConstantType::Double: return new ConstantDoubleInfo;
        case ConstantType::Utf8: return new ConstantUtf8Info;
        case ConstantType::String: return new ConstantStringInfo(cp);
        case ConstantType::Class: return new ConstantClassInfo(cp);
        case ConstantType::Fieldref: return new ConstantFieldrefInfo(cp);
        case ConstantType::Methodref: return new ConstantMethodrefInfo(cp);
        case ConstantType::InterfaceMethodref: return new ConstantInterfaceMethodrefInfo(cp);
        case ConstantType::NameAndType: return new ConstantNameAndTypeInfo;
        // case ConstantType::MethodType: return new ConstantMethodTypeInfo;
        // case ConstantType::MethodHandle: return new ConstantMethodHandleInfo;
        // case ConstantType::InvokeDynamic: return new ConstantInvokeDynamicInfo;
        default:
            cerr << "java.lang.ClassFormatError: constant pool tag!" << endl;
            exit(1);
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