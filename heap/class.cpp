#include "class.h"
using namespace std;

Class::Class(classfile::Classfile& cf): constantPool(ConstantPool(this, cf.getConstantPool())){
    accessFlag = cf.getAccessFlags();
    name = cf.getClassName();
    superClassName = cf.getSuperClassName();
    interfaceNames = cf.getInterfacesNames();
    
    fields = getFields(this, cf.getFields());
    methods = getMethods(this, cf.getMethods());
}

vector<Field*> getFields(Class* _class, const vector<classfile::MemberInfo*>& cfFields){
    vector<Field*> fields(cfFields.size());
    for(size_t i = 0; i < cfFields.size(); i++){
        fields[i] = new Field(_class, cfFields[i]);
    }
    return fields;
}

vector<Method*> getMethods(Class* _class, const std::vector<classfile::MemberInfo*>& cfMethods){
    vector<Method*> methods(cfMethods.size());
    for(size_t i = 0; i < cfMethods.size(); i++){
        classfile::MemberInfo* method = cfMethods[i];
        methods[i] = new Method(_class, method);
        classfile::CodeAttribute* codeAttr = method->getCodeAttribute();
        methods[i]->code = codeAttr->code;
        methods[i]->maxLocals = codeAttr->maxLocals;
        methods[i]->maxStack = codeAttr->maxStack;
    }
    return methods;
}

ConstantPool::ConstantPool(Class* _class, classfile::ConstantPool& cp){
    unsigned int cpCount = cp.size();
    consts = vector<Constant>(cpCount);
    for(size_t i = 0; i < cpCount; i++){
        classfile::ConstantInfo* cpInfo = cp[i];
        switch(cpInfo->getTag()){
            // TODO
            case classfile::ConstantType::Integer: break;
        }
    }
}