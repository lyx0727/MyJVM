#include "constant_pool.h"
#include "sym_ref.h"
using namespace std;


ConstantPool::ConstantPool(Class* _class, classfile::ConstantPool& cp): _class(_class){
    unsigned int cpCount = cp.size();
    consts = vector<Constant>(cpCount);
    for(size_t i = 1; i < cpCount; i++){
        classfile::ConstantInfo* cpInfo = cp[i];
        switch(cpInfo->getTag()){
            case classfile::ConstantType::Integer: 
                consts[i] = Constant(dynamic_cast<classfile::ConstantIntegerInfo*>(cpInfo)->val);
                break;
            case classfile::ConstantType::Float:
                consts[i] = Constant(dynamic_cast<classfile::ConstantFloatInfo*>(cpInfo)->val);
                break;
            case classfile::ConstantType::Long:
                consts[i] = Constant(dynamic_cast<classfile::ConstantLongInfo*>(cpInfo)->val);
                i++;
                break;
            case classfile::ConstantType::Double:
                consts[i] = Constant(dynamic_cast<classfile::ConstantDoubleInfo*>(cpInfo)->val);
                i++;
                break;
            case classfile::ConstantType::String:
                consts[i] = Constant(dynamic_cast<classfile::ConstantStringInfo*>(cpInfo)->getString());
                break;
            case classfile::ConstantType::Class:
                consts[i] = Constant((Ref)new ClassRef(this, dynamic_cast<classfile::ConstantClassInfo*>(cpInfo)));
                break;
            case classfile::ConstantType::Fieldref:
                consts[i] = Constant((Ref)new FieldRef(this, dynamic_cast<classfile::ConstantFieldrefInfo*>(cpInfo)));
                break;
            case classfile::ConstantType::Methodref:
                consts[i] = Constant((Ref)new MemberRef(this, dynamic_cast<classfile::ConstantMemberrefInfo*>(cpInfo)));
                break;
            case classfile::ConstantType::InterfaceMethodref:
                consts[i] = Constant((Ref)new InterfaceMethodRef(this, dynamic_cast<classfile::ConstantInterfaceMethodrefInfo*>(cpInfo)));
                break;
        }
    }
}

const string Constant::toString() const {
    if(type == ConstantType::Int){
        return std::to_string(getVal<int>());
    }
    if(type == ConstantType::Float){
        return std::to_string(getVal<float>());
    }
    if(type == ConstantType::Double){
        return std::to_string(getVal<double>());
    }
    if(type == ConstantType::Long){
        return std::to_string(getVal<long>());
    }
    if(type == ConstantType::String){
        return std::string((char*)getVal<Ref>());
    }
    return std::to_string((unsigned long)getVal<Ref>());
}   