#include "attribute_info.h"
using namespace std;

namespace classfile{
AttributeInfo* getAttributeInfo(const std::string& attrName, uint32_t attrlen, ConstantPool& cp){
    uint8_t tag;
    if(AttributeTypeMap.count(attrName)){
        tag = AttributeTypeMap.at(attrName);
    }
    else{
        return new UnparsedAttribute(attrName, attrlen);
    }
    
    switch (tag){
        case AttributeType::Code:  return new CodeAttribute(cp);
        case AttributeType::ConstantValue:  return new ConstantValueAttribute;
        case AttributeType::Deprecated:  return new DeprecatedAttribute;
        case AttributeType::Exceptions:  return new ExceptionsAttribute;
        case AttributeType::LineNumberTable:  return new LineNumberTableAttribute;
        case AttributeType::LocalVariableTable:  return new LocalVariableTableAttribute;
        case AttributeType::SourceFile:  return new SourceFileAttribute(cp);
        case AttributeType::Synthetic:  return new SyntheticAttribute;
        default: return new UnparsedAttribute(attrName, attrlen);
    }
}

vector<AttributeInfo*> readAttributes(ClassReader& cr, ConstantPool& cp){
    uint16_t n = cr.readUint16();
    vector<AttributeInfo*> attrs(n);
    for(uint16_t i = 0; i < n; i++){
        attrs[i] = readAttribute(cr, cp);
    }
    return attrs;
}

AttributeInfo* readAttribute(ClassReader& cr,  ConstantPool& cp){
    uint16_t nameIndex = cr.readUint16();
    string name = cp.getUtf8(nameIndex);
    uint32_t len = cr.readUint32();
    AttributeInfo* attr = getAttributeInfo(name, len, cp);
    attr->readInfo(cr);
    return attr;
}

vector<ExceptionTableEntry> readExceptionTable(ClassReader& cr){
    uint16_t n = cr.readUint16();
    vector<ExceptionTableEntry> exceptionTable(n);
    for(uint16_t i = 0; i < n; i++){
        exceptionTable[i] = ExceptionTableEntry(
            cr.readUint16(),    // startPc 
            cr.readUint16(),    // endPc
            cr.readUint16(),    // handlerPc
            cr.readUint16()     // catchType
        );
    }
    return exceptionTable;
}

vector<LineNumberTableEntry> readLineNumberTable(ClassReader& cr){
    uint16_t n = cr.readUint16();
    vector<LineNumberTableEntry> lineNumberTable(n);
    for(uint16_t i = 0; i < n; i++){
        lineNumberTable[i] = LineNumberTableEntry(
            cr.readUint16(),    // startPc 
            cr.readUint16()     // lineNumber
        );
    }
    return lineNumberTable;
}

vector<LocalVariableTableEntry> readLocalVariableTable(ClassReader& cr){
    uint16_t n = cr.readUint16();
    vector<LocalVariableTableEntry> localVariableTable(n);
    for(uint16_t i = 0; i < n; i++){
        localVariableTable[i] = LocalVariableTableEntry(
            cr.readUint16(),    // startPc 
            cr.readUint16(),    // length
            cr.readUint16(),    // nameIndex
            cr.readUint16(),    // descriptorIndex
            cr.readUint16()     // index
        );
    }
    return localVariableTable;
}

LineNumberTableAttribute* CodeAttribute::getLineNumberTableAttribute() const{
    for(AttributeInfo* attr : attributes){
        string name = attr->getName();
        if(AttributeTypeMap.count(name)){
            uint8_t type = AttributeTypeMap.at(name);
            if(type == AttributeType::LineNumberTable){
                return dynamic_cast<LineNumberTableAttribute*>(attr);
            }
        }
    }
    return nullptr;
}

int LineNumberTableAttribute::getLineNumber(int pc){
    for(int i = lineNumberTable.size() - 1; i >= 0; i--){
        LineNumberTableEntry& entry = lineNumberTable[i];
        if(pc >= entry.startPc){
            return entry.lineNumber;
        }
    }
    return -1;
}
}