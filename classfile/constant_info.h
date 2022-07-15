#ifndef CONSTANT_INFO_GUARD
#define CONSTANT_INFO_GUARD
#include "class_reader.h"
#include "constant_pool.h"

class ConstantPool;

struct ConstantInfo{};

const ConstantInfo readConstantInfo(ClassReader& cr, ConstantPool& cp);

#endif