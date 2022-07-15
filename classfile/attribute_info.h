#ifndef ATTRIBUTE_INFO_GUARD
#define ATTRIBUTE_INFO_GUARD
#include <vector>
#include "class_reader.h"
#include "constant_pool.h"

class ClassReader;
class ConstantPool;

struct AttributeInfo{};

const std::vector<AttributeInfo> readAttributes(ClassReader& cr,  ConstantPool& cp);

#endif