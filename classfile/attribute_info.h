#ifndef ATTRIBUTE_INFO_GUARD
#define ATTRIBUTE_INFO_GUARD
#include <vector>
#include <map>
#include "class_reader.h"
#include "constant_pool.h"

enum AttributeType{
    Code,
    ConstantValue,
    Deprecated,
    Exceptions,
    LineNumberTable,
    LocalVariableTable,
    SourceFile,
    Synthetic
};

const std::map<std::string, uint8_t> AttributeNameMap{
    { "Code",               AttributeType::Code },
    { "ConstantValue",      AttributeType::ConstantValue },
    { "Deprecated",         AttributeType::Deprecated },
    { "Exceptions",         AttributeType::Exceptions },
    { "LineNumberTable",    AttributeType::LineNumberTable },
    { "LocalVariableTable", AttributeType::LocalVariableTable },
    { "SourceFile",         AttributeType::SourceFile },
    { "Synthetic",          AttributeType::Synthetic },
};

class ClassReader;
class ConstantPool;

struct AttributeInfo{
    virtual void readInfo(ClassReader& cr) = 0;
    virtual ~AttributeInfo(){}
};

std::vector<AttributeInfo*> readAttributes(ClassReader& cr,  ConstantPool& cp);
AttributeInfo* readAttribute(ClassReader& cr,  ConstantPool& cp);
AttributeInfo* getAttributeInfo(const std::string& attrName, uint32_t attrlen, ConstantPool& cp);


struct MarkerAttribute : public AttributeInfo{
    void readInfo(ClassReader& cr){
        // only a mark, read nothing
    }
};
struct DeprecatedAttribute : public MarkerAttribute{};
struct SyntheticAttribute : public MarkerAttribute{};

struct SourceFileAttribute : public AttributeInfo{
    ConstantPool& cp;
    uint16_t sourceFileIndex;
    SourceFileAttribute(ConstantPool& cp): cp(cp){}
    void readInfo(ClassReader& cr){
        sourceFileIndex = cr.readUint16();
    }
    const std::string getFileName() const {
        return cp.getUtf8(sourceFileIndex);
    }
};

struct ConstantValueAttribute : public AttributeInfo{
    uint16_t constantValueIndex;
    void readInfo(ClassReader& cr){
        constantValueIndex = cr.readUint16();
    }
};

struct ExceptionTableEntry{
    uint16_t startPc;
    uint16_t endPc;
    uint16_t handlerPc;
    uint16_t catchType;
    ExceptionTableEntry(){}
    ExceptionTableEntry(uint16_t startPc, uint16_t endPc, uint16_t handlerPc, uint16_t catchType)
                    : startPc(startPc), endPc(endPc), handlerPc(handlerPc), catchType(catchType){}
};
std::vector<ExceptionTableEntry> readExceptionTable(ClassReader& cr);

struct CodeAttribute : public AttributeInfo{
    ConstantPool&                    cp;
    uint16_t                         maxStack;
    uint16_t                         maxLocals;
    std::vector<Byte>                code;
    std::vector<AttributeInfo*>      attributes;
    std::vector<ExceptionTableEntry> exceptionTable;
    CodeAttribute(ConstantPool& cp): cp(cp){}
    void readInfo(ClassReader& cr){
        maxStack = cr.readUint16();
        maxLocals = cr.readUint16();
        uint32_t codeLen = cr.readUint32();
        code = cr.readBytes(codeLen);
        exceptionTable = readExceptionTable(cr);
        attributes = readAttributes(cr, cp);
    }
};

struct ExceptionsAttribute : public AttributeInfo{
    std::vector<uint16_t> exceptionIndexTable;
    void readInfo(ClassReader& cr){
        exceptionIndexTable = cr.readUint16s();
    }
};

struct LineNumberTableEntry{
    uint16_t startPc;
    uint16_t lineNumber;
    LineNumberTableEntry(){}
    LineNumberTableEntry(uint16_t startPc, uint16_t lineNumber)
                    : startPc(startPc), lineNumber(lineNumber){}
};
std::vector<LineNumberTableEntry> readLineNumberTable(ClassReader& cr);

struct LineNumberTableAttribute : public AttributeInfo{
    std::vector<LineNumberTableEntry> lineNumberTable;
    void readInfo(ClassReader& cr){
        lineNumberTable = readLineNumberTable(cr);
    }
};

struct LocalVariableTableEntry {
	uint16_t startPc;
	uint16_t length;   
	uint16_t nameIndex;
	uint16_t descriptorIndex; 
	uint16_t index;
    LocalVariableTableEntry(){}
    LocalVariableTableEntry(uint16_t startPc, uint16_t length, uint16_t nameIndex, uint16_t descriptorIndex, uint16_t index)
    :startPc(startPc), length(length), nameIndex(nameIndex), descriptorIndex(descriptorIndex), index(index){} 
};
std::vector<LocalVariableTableEntry> readLocalVariableTable(ClassReader& cr);

struct LocalVariableTableAttribute : public AttributeInfo{
    std::vector<LocalVariableTableEntry> localVariableTable;
    void readInfo(ClassReader& cr){
        localVariableTable = readLocalVariableTable(cr);
    }
};

struct UnparsedAttribute : public AttributeInfo{
    std::string       name;
    uint32_t          length;
    std::vector<Byte> info;
    UnparsedAttribute(const std::string& name, uint32_t length)
        :name(name), length(length){} 
    void readInfo(ClassReader& cr){
        info = cr.readBytes(length);
    }
};
#endif