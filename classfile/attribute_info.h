#ifndef ATTRIBUTE_INFO_GUARD
#define ATTRIBUTE_INFO_GUARD
#include <vector>
#include "class_reader.h"
#include "constant_pool.h"

class ClassReader;
class ConstantPool;

struct AttributeInfo{
    virtual void readInfo(ClassReader& cr) = 0;
    virtual ~AttributeInfo(){}
};

const std::vector<AttributeInfo*> readAttributes(ClassReader& cr,  ConstantPool& cp);
const AttributeInfo* readAttribute(ClassReader& cr,  ConstantPool& cp);

#endif