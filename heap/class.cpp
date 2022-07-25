#include "class.h"
#include "sym_ref.h"
using namespace std;

Class::Class(classfile::Classfile& cf)
    : constantPool(new ConstantPool(this, cf.getConstantPool())), classLoader(nullptr), superClass(nullptr){
    accessFlag = cf.getAccessFlags();
    name = cf.getClassName();
    superClassName = cf.getSuperClassName();
    interfaceNames = cf.getInterfacesNames();
    fields = getFields(this, cf.getFields());
    methods = getMethods(this, cf.getMethods());
}

Class::~Class(){ 
    delete constantPool;     
}

Method* Class::getStaticMethod(const string& name, const string& descriptor) const{
    for(Method* method : methods){
        if(method->isStatic() && method->name == name && method->descriptor == descriptor)
            return method;
    }
    return nullptr;
}

const string Class::getPackageName() const {
    size_t n = name.length();
    int i;
    for(i = n - 1; i > 0; i--)
        if(name[i] == '/') break;
    if(i > 0){
        return name.substr(0, i - 1);
    }
    return "";
}

bool Class::isSubClassOf(Class* c) const {
    for(const Class* t = this; t; t = superClass){
        if(t == c) 
            return true;
    }
    return false;
}

bool Class::isSubInterfaceOf(Class* iface) const {
    for(const Class* const i : interfaces){
        if(i == iface || i->isSubClassOf(iface))
            return true;
    }
    return false;
}

bool Class::isImplements(Class* iface) const {
    for(const Class* t = this; t; t = superClass){
        for(Class* i : t->interfaces){
            if(i == iface || i->isSubInterfaceOf(iface))
                return true;
        }
    }
    return false;
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
    }
    return methods;
}

Field* Class::lookupField(const string& name, const string& descriptor) const {
    for(Field* field : fields){
        if(field->name == name && field->descriptor == descriptor){
            return field;
        }
    }
    for(Class* interface : interfaces){
        Field* field = interface->lookupField(name, descriptor);
        if(field != nullptr){
            return field;
        }
    }
    if(superClass != nullptr){
        return superClass->lookupField(name, descriptor);
    }
    return nullptr;
}