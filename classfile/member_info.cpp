#include "member_info.h"
using namespace std;

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