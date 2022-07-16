#include "classfile.h"
#include <iostream>
using namespace std;

void ClassFile::read(ClassReader& reader){
    readAndCheckMagic(reader);
    readAndCheckVersion(reader);

    readConstantPool(reader, constantPool);

    accessFlags = reader.readUint16();
    thisClass = reader.readUint16();
    superClass = reader.readUint16();
    interfaces = reader.readUint16s();

    fields = readMembers(reader, constantPool);
    methods = readMembers(reader, constantPool);
    attributes = readAttributes(reader, constantPool);
}

void ClassFile::readAndCheckMagic(ClassReader& reader){
    uint32_t magic = reader.readUint32();
    if(magic != 0xCAFEBABE){
        cerr << "java.lang.ClassFormatError: magic!" << endl;
        exit(1);
    }
}

void ClassFile::readAndCheckVersion(ClassReader& reader){
    minorVersion = reader.readUint16();
    majorVersion = reader.readUint16();
    switch (majorVersion){
    case 45:
        return;
    case 46:
    case 47:
    case 48:
    case 49:    
    case 50:    
    case 51:    
    case 52:    
        if(minorVersion == 0) return;
    }
    cerr << "java.lang.UnsupportedClassVersionError!" << endl;
    exit(1);
}

const vector<string> ClassFile::getInterfacesNames() const {
    size_t n = interfaces.size();
    vector<string> interfaceNames(n);
    for(size_t i = 0; i < n; i++){
        interfaceNames[i] = constantPool.getClassName(interfaces[i]);
    }
    return interfaceNames;
}

const string ClassFile::toString() const {
    string str = "[version]\t" + to_string(getMajorVersion()) + "." + to_string(getMinorVersion())
        + "\n" + "[access flag]\t" + to_string(getAccessFlags())
        + "\n" + "[this class]\t" + getClassName() 
        + "\n" + "[super class]\t" + getSuperClassName()
        + "\n" + "[interfaces]\n";
    vector<string> interfaceNames = getInterfacesNames();
    for(string& interfaceName : interfaceNames){
        str += "\t" + interfaceName + "\n";
    }
    vector<MemberInfo*> fields = getFields();
    str += "[fileds count]\t" + to_string(fields.size()) + "\n";
    for(MemberInfo* field : fields){
        str += "\t" + field->toString() + "\n";
    }
    vector<MemberInfo*> methods = getMethods();
    str += "[methods count]\t" + to_string(methods.size()) + "\n";
    for(MemberInfo* method : methods){
        str += "\t" + method->toString() + "\n";
    }
    return str;
}

ClassFile::~ClassFile(){
    for(MemberInfo* member : fields){
        delete member;
        // cout << member << ": " << member->getName() << endl;
    }
    for(MemberInfo* member : methods){
        delete member;
        // cout << member << ": " << member->getName() << endl;
    }
    for(AttributeInfo* attr : attributes){
        delete attr;
    }
}
