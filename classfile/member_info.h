#ifndef MEMBER_INFO_GUARD
#define MEMBER_INFO_GUARD
#include <vector>
#include <cstdint>
#include "class_reader.h"
#include "constant_pool.h"
#include "attribute_info.h"

struct MemberInfo{
private:
    ConstantPool&               cp;
    uint16_t                    accessFlags;
    uint16_t                    nameIndex;
    uint16_t                    descriptorIndex;
    std::vector<AttributeInfo*> attributes;
public:
    MemberInfo(ConstantPool& cp, uint16_t af, uint16_t ni, uint16_t di, const std::vector<AttributeInfo*>& attrs)
            :cp(cp), accessFlags(af), nameIndex(ni), descriptorIndex(di), attributes(attrs) {}
    ~MemberInfo();
    MemberInfo& operator=(const MemberInfo& rhs) = delete;
    const uint16_t getAccessFlags() const { return accessFlags; }
    const std::string getName() const { return cp.getUtf8(nameIndex); } 
    const std::string getDescriptor() const { return cp.getUtf8(descriptorIndex); } 
    const std::string toString() const { return getName(); }
};

std::vector<MemberInfo*> readMembers(ClassReader& cr, ConstantPool& cp);

MemberInfo* readMember(ClassReader& cr, ConstantPool& cp);

#endif