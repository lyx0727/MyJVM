#ifndef CLASS_READER_GUARD
#define CLASS_READER_GUARD
#include <vector>
#include <cstdint>
#include "../utils/utils.h"

namespace classfile{
class ClassReader{
private:
    std::vector<Byte> bytes;
    size_t cur;
public:
    ClassReader(const std::vector<Byte>& classData):bytes(classData), cur(0){}
    const uint8_t readUint8();
    const uint16_t readUint16();
    const uint32_t readUint32();
    const uint64_t readUint64();
    const std::vector<uint16_t> readUint16s();
    const std::vector<Byte> readBytes(uint32_t n);
};
}

#endif