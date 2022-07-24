#include "member_info.h"
using namespace std;

namespace classfile{
MemberInfo::~MemberInfo(){ 
    for(AttributeInfo* attr : attributes){
        delete attr;
    }
}

vector<MemberInfo*> readMembers(ClassReader& cr, ConstantPool& cp){
    uint16_t n = cr.readUint16();
    vector<MemberInfo*> memberInfos(n);
    for(uint16_t i = 0; i < n; i++){
        memberInfos[i] = readMember(cr, cp);
    }
    return memberInfos;
}

MemberInfo* readMember(ClassReader& cr, ConstantPool& cp){
    return new MemberInfo(
        cp,                         // cp
        cr.readUint16(),            // accessFlags
        cr.readUint16(),            // nameIndex
        cr.readUint16(),            // descriptorIndex
        readAttributes(cr, cp)      // attributes
    );
}

const string MemberInfo::toString() const {
    string str = getName() + ": " + getDescriptor();
    for(AttributeInfo* attr : attributes){
        str += "\n\t\t" + attr->toString();  
    }
    return str;
}

CodeAttribute* MemberInfo::getCodeAttribute() const {
    for(AttributeInfo* attr : attributes){
        if(AttributeTypeMap.count(attr->getName())){
            uint8_t type = AttributeTypeMap.at(attr->getName());
            switch (type){
                case AttributeType::Code:
                    return dynamic_cast<CodeAttribute*>(attr);
            }
        }
    }
    return nullptr;
}

ConstantValueAttribute* MemberInfo::getConstantValueAttribute() const {
    for(AttributeInfo* attr : attributes){
        uint8_t type = AttributeTypeMap.at(attr->getName());
        switch (type){
            case AttributeType::Code:
                return dynamic_cast<ConstantValueAttribute*>(attr);
        }
    }
    return nullptr;
}
}