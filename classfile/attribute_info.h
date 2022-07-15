#ifndef ATTRIBUTE_INFO_GUARD
#define ATTRIBUTE_INFO_GUARD
#include <vector>
#include "class_reader.h"

struct AttributeInfo{};

const std::vector<AttributeInfo> readAttributes(ClassReader& cr);

#endif