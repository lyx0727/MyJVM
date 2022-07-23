#ifndef HEAP_CONSTANT_POOL_GUARD
#define HEAP_CONSTANT_POOL_GUARD
#include "class.h"

class Class;
struct Field;
struct Method;

union Constant{
    int intVal;
    long longVal;
    float floatVal;
    double doubleVal;
    void* ref;
};

class ConstantPool{
private:
    std::vector<Constant> consts;
public:
    Class* _class;
    ConstantPool(Class* _class, classfile::ConstantPool& cp);
    Constant getConstant(unsigned int index) const { return consts[index]; }
};

#endif