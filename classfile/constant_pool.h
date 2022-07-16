#ifndef CONSTANT_POOL_GUARD
#define CONSTANT_POOL_GUARD
#include <string>
#include <cstdint>
#include "class_reader.h"
#include "constant_info.h"

// forward declaraction to replace defination 
class ConstantInfo;
struct ConstantNameAndTypeInfo;
struct ConstantClassInfo;
struct ConstantUtf8Info;

class ConstantPool{
private:
    std::vector<ConstantInfo*> constantInfos;
public:
    ConstantPool(uint16_t n = 0): constantInfos(std::vector<ConstantInfo*>(n)){}
    ~ConstantPool();
    ConstantPool& operator=(const ConstantPool& rhs) = delete;

    void resize(size_t n){ constantInfos.resize(n); }

    ConstantInfo*& operator[](uint16_t index){ return constantInfos[index]; }

    // 'ConstantInfo' appears as pointer type so that its defination is not necessary
    // a trick to avoid compile dependency
    const ConstantInfo* getConstantInfo(uint16_t index) const { return constantInfos[index]; }

    const std::pair<std::string, std::string> getNameAndType(uint16_t index) const;

    const std::string getClassName(uint16_t index) const;
    
    const std::string getUtf8(uint16_t index) const;
};

void readConstantPool(ClassReader& cr, ConstantPool& cp);

#endif