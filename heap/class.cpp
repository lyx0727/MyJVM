#include "class.h"
#include "sym_ref.h"
using namespace std;

Class::Class(classfile::Classfile& cf){
    accessFlag = cf.getAccessFlags();
    name = cf.getClassName();
    superClassName = cf.getSuperClassName();
    interfaceNames = cf.getInterfacesNames();
    
    constantPool = new ConstantPool(this, cf.getConstantPool());
    fields = getFields(this, cf.getFields());
    methods = getMethods(this, cf.getMethods());
}

const string Class::getPackageName() const{
    size_t n = name.length();
    int i;
    for(i = n - 1; i > 0; i--)
        if(name[i] == '/') break;
    if(i > 0){
        return name.substr(0, i - 1);
    }
    return "";
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
            case classfile::ConstantType::Integer: 
                consts[i].intVal = dynamic_cast<classfile::ConstantIntegerInfo*>(cpInfo)->val;
                break;
            case classfile::ConstantType::Float:
                consts[i].floatVal = dynamic_cast<classfile::ConstantFloatInfo*>(cpInfo)->val;
                break;
            case classfile::ConstantType::Long:
                consts[i].longVal = dynamic_cast<classfile::ConstantLongInfo*>(cpInfo)->val;
                i++;
                break;
            case classfile::ConstantType::Double:
                consts[i].doubleVal = dynamic_cast<classfile::ConstantDoubleInfo*>(cpInfo)->val;
                i++;
                break;
            case classfile::ConstantType::String:
                strcpy((char*)consts[i].ref, dynamic_cast<classfile::ConstantStringInfo*>(cpInfo)->getString().c_str());
                break;
            case classfile::ConstantType::Class:
                consts[i].ref = new ClassRef(this, dynamic_cast<classfile::ConstantClassInfo*>(cpInfo));
                break;
            case classfile::ConstantType::Fieldref:
                consts[i].ref = new FieldRef(this, dynamic_cast<classfile::ConstantFieldrefInfo*>(cpInfo));
                break;
            case classfile::ConstantType::Methodref:
                consts[i].ref = new MemberRef(this, dynamic_cast<classfile::ConstantMemberrefInfo*>(cpInfo));
                break;
            case classfile::ConstantType::InterfaceMethodref:
                consts[i].ref = new InterfaceMethodRef(this, dynamic_cast<classfile::ConstantInterfaceMethodrefInfo*>(cpInfo));
                break;
        }
    }
}

Field* lookupField(Class* _class, const string& name, const string& descriptor){
    for(Field* field : _class->fields){
        if(field->name == name && field->descriptor == descriptor){
            return field;
        }
    }
    for(Class* interface : _class->interfaces){
        Field* field = lookupField(interface, name, descriptor);
        if(field != nullptr){
            return field;
        }
    }
    if(_class->superClass != nullptr){
        return lookupField(_class->superClass, name, descriptor);
    }
    return nullptr;
}