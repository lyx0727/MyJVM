#ifndef CONSTANT_POOL_GUARD
#define CONSTANT_POOL_GUARD
#include <string>
#include <cstdint>
#include "class_reader.h"
#include "constant_info.h"

class ConstantInfo;

class ConstantPool{
private:
    std::vector<ConstantInfo> constantInfos;
public:
    ConstantPool(uint16_t n = 0): constantInfos(std::vector<ConstantInfo>(n)){}

    ConstantInfo& operator[](uint16_t index){ return constantInfos[index]; }

    const ConstantInfo getConstantInfo(uint16_t index) const ;
    // TODO
    const std::pair<std::string, std::string> getNameAndType(uint16_t index) const { return std::make_pair("", ""); }
    // TODO
    const std::string getClassName(uint16_t index) const { return ""; } 
    // TODO
    const std::string getUtf8(uint16_t index) const { return ""; }
};

const ConstantPool readConstantPool(ClassReader& cr);

#endif