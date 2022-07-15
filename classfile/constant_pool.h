#ifndef CONSTANT_POOL_GUARD
#define CONSTANT_POOL_GUARD
#include <string>
#include <cstdint>
#include "class_reader.h"

class ConstantPool{
private:
public:
    const std::string getClassName(uint16_t index) const;
};

const ConstantPool readConstantPool(ClassReader& cr);

#endif