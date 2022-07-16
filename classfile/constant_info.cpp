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
        case ConstantType::CONSTANT_Integer: return new ConstantIntegerInfo;
        case ConstantType::CONSTANT_Float: return new ConstantFloatInfo;
        case ConstantType::CONSTANT_Long: return new ConstantLongInfo;
        case ConstantType::CONSTANT_Double: return new ConstantDoubleInfo;
        case ConstantType::CONSTANT_Utf8: return new ConstantUtf8Info;
        case ConstantType::CONSTANT_String: return new ConstantStringInfo(cp);
        case ConstantType::CONSTANT_Class: return new ConstantClassInfo(cp);
        case ConstantType::CONSTANT_Fieldref: return new ConstantFieldrefInfo(cp);
        case ConstantType::CONSTANT_Methodref: return new ConstantMethodrefInfo(cp);
        case ConstantType::CONSTANT_InterfaceMethodref: return new ConstantInterfaceMethodrefInfo(cp);
        case ConstantType::CONSTANT_NameAndType: return new ConstantNameAndTypeInfo;
        case ConstantType::CONSTANT_MethodHandle: return new ConstantMethodHandleInfo;
        case ConstantType::CONSTANT_InvokeDynamic: return new ConstantInvokeDynamicInfo;
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

