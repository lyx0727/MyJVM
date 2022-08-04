#include "constant_pool.h"
#include "sym_ref.h"
#include <fstream>
using namespace std;

ConstantPool::ConstantPool(Class* _class, classfile::ConstantPool& cp): _class(_class){
    unsigned int cpCount = cp.size();
    consts = vector<Constant>(cpCount, Constant());
    for(size_t i = 1; i < cpCount; i++){
        classfile::ConstantInfo* cpInfo = cp[i];
        switch(cpInfo->getTag()){
            case classfile::ConstantInfoType::Integer: 
                consts[i] = Constant(dynamic_cast<classfile::ConstantIntegerInfo*>(cpInfo)->val);
                break;
            case classfile::ConstantInfoType::Float:
                consts[i] = Constant(dynamic_cast<classfile::ConstantFloatInfo*>(cpInfo)->val);
                break;
            case classfile::ConstantInfoType::Long:
                consts[i] = Constant(dynamic_cast<classfile::ConstantLongInfo*>(cpInfo)->val);
                i++;
                break;
            case classfile::ConstantInfoType::Double:
                consts[i] = Constant(dynamic_cast<classfile::ConstantDoubleInfo*>(cpInfo)->val);
                i++;
                break;
            case classfile::ConstantInfoType::String:  
                consts[i] = Constant(dynamic_cast<classfile::ConstantStringInfo*>(cpInfo)->getString());
                break;
            case classfile::ConstantInfoType::Class:
                consts[i] = Constant(new ClassRef(this, dynamic_cast<classfile::ConstantClassInfo*>(cpInfo)));
                break;
            case classfile::ConstantInfoType::Fieldref:
                consts[i] = Constant(new FieldRef(this, dynamic_cast<classfile::ConstantFieldrefInfo*>(cpInfo)));
                break;
            case classfile::ConstantInfoType::Methodref:
                consts[i] = Constant(new MethodRef(this, dynamic_cast<classfile::ConstantMethodrefInfo*>(cpInfo)));
                break;
            case classfile::ConstantInfoType::InterfaceMethodref:
                consts[i] = Constant(new InterfaceMethodRef(this, dynamic_cast<classfile::ConstantInterfaceMethodrefInfo*>(cpInfo)));
                break;
        }
    }
}

const string Constant::toString() const {
    switch(type){
        case ConstantType::Int: return to_string(getVal<int>());
        case ConstantType::Float: return to_string(getVal<float>());
        case ConstantType::Double: return to_string(getVal<double>());
        case ConstantType::Long: return to_string(getVal<long>());
        case ConstantType::String: return getVal<string>();
        case ConstantType::ClassRef: return getVal<ClassRef*>()->className;
        case ConstantType::MethodRef: 
            return getVal<MethodRef*>()->className + "." + getVal<MethodRef*>()->name + getVal<MethodRef*>()->descriptor;
        case ConstantType::FieldRef: 
            return getVal<FieldRef*>()->className + "." + getVal<FieldRef*>()->name + " " + getVal<FieldRef*>()->descriptor;   
        case ConstantType::InterfaceMethodRef: 
            return getVal<InterfaceMethodRef*>()->className + "." + getVal<InterfaceMethodRef*>()->name + getVal<InterfaceMethodRef*>()->descriptor; 
        case ConstantType::Null: return "Null";
    }
    return to_string((unsigned long)getVal<Ref>());
}   

ConstantPool::~ConstantPool(){
    for(Constant& c : consts){
        Ref r = c.val.r;
        if(r){
            switch(c.type){
                case ConstantType::String:             delete (char*)r; break;
                case ConstantType::ClassRef:           delete (ClassRef*)r; break;
                case ConstantType::MethodRef:          delete (MethodRef*)r; break;
                case ConstantType::FieldRef:           delete (FieldRef*)r; break;
                case ConstantType::InterfaceMethodRef: delete (InterfaceMethodRef*)r; break;
            }
            c.val.r = nullptr;
        }
        c.type = ConstantType::Null;
    }
}

ClassRef* ConstantPool::getCatchType(unsigned int index){
    if(index == 0){
        return nullptr;
    }
    return (ClassRef*)getConstant(index).getVal<Ref>();
}