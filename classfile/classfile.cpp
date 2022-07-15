#include "classfile.h"
using namespace std;

void ClassFile::read(ClassReader& reader){
    readAndCheckMagic(reader);
    readAndCheckVersion(reader);

    constantPool = readConstantPool(reader);

    accessFlags = reader.readUint16();
    thisClass = reader.readUint16();
    superClass = reader.readUint16();
    interfaces = reader.readUint16s();

    fields = readMembers(reader);
    methods = readMembers(reader);
    attributes = readAttributes(reader);
}

void ClassFile::readAndCheckMagic(ClassReader& reader){
    // TODO
}

void ClassFile::readAndCheckVersion(ClassReader& reader){
    // TODO
}

const vector<string> ClassFile::getInterfacesNames() const {
    size_t n = interfaces.size();
    vector<string> interfaceNames(n);
    for(size_t i = 0; i < n; i++){
        interfaceNames[i] = constantPool.getClassName(interfaces[i]);
    }
    return interfaceNames;
}
