#include "class.h"
#include "sym_ref.h"
using namespace std;

Class::Class(classfile::Classfile& cf)
    : constantPool(new ConstantPool(this, cf.getConstantPool())), classLoader(nullptr), superClass(nullptr){
    accessFlag = cf.getAccessFlags();
    name = cf.getClassName();
    superClassName = cf.getSuperClassName();
    interfaceNames = cf.getInterfacesNames();
    getFields(cf.getFields());
    getMethods(cf.getMethods());
}

Class::~Class(){ 
    delete constantPool; 
    delete staticVars;
}

Object* Class::newArray(unsigned int count){
    if(!isArray()){
        cerr << "Not array class: " + name << endl;
        exit(1);
    }
    if(name == "[Z" || name == "[B") return new Object(this, ObjectType::ByteArr, count);
    else if(name == "[C") return new Object(this, ObjectType::CharArr, count);
    else if(name == "[S") return new Object(this, ObjectType::ShortArr, count);
    else if(name == "[I") return new Object(this, ObjectType::IntArr, count);
    else if(name == "[F") return new Object(this, ObjectType::FloatArr, count);
    else if(name == "[J") return new Object(this, ObjectType::LongArr, count);
    else if(name == "[D") return new Object(this, ObjectType::DoubleArr, count);
    return new Object(this, ObjectType::RefArr, count);
}
Object* Class::newObject(){
    return new Object(this);
}

Class* Class::getArrayClass(){ return classLoader->loadClass("[" + toDescriptor(name) ); }

const string toDescriptor(const string& className){
    if(className[0] == '[') return className;
    if(primitiveTypeMap.count(className)){
        return primitiveTypeMap.at(className);
    }
    return "L" + className + ";";
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

bool Class::isSubClassOf(const Class* c) const {
    for(const Class* t = this; t; t = superClass){
        if(t == c) 
            return true;
    }
    return false;
}

bool Class::isSuperClassOf(const Class* c) const{
    return c->isSubClassOf(this);
}

bool Class::isSubInterfaceOf(const Class* iface) const {
    for(const Class* const i : interfaces){
        if(i == iface || i->isSubClassOf(iface))
            return true;
    }
    return false;
}

bool Class::isImplements(const Class* iface) const {
    for(const Class* t = this; t; t = superClass){
        for(Class* i : t->interfaces){
            if(i == iface || i->isSubInterfaceOf(iface))
                return true;
        }
    }
    return false;
}

vector<Field*> Class::getFields(const vector<classfile::MemberInfo*>& cfFields){
    fields = vector<Field*>(cfFields.size());
    for(size_t i = 0; i < cfFields.size(); i++){
        fields[i] = new Field(this, cfFields[i]);
    }
    return fields;
}

vector<Method*> Class::getMethods(const std::vector<classfile::MemberInfo*>& cfMethods){
    methods = vector<Method*>(cfMethods.size());
    for(size_t i = 0; i < cfMethods.size(); i++){
        classfile::MemberInfo* method = cfMethods[i];
        methods[i] = new Method(this, method);
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

Method* Class::lookupMethod(const string& name, const string& descriptor) const {
    Method* method = lookupMethodInClass(name, descriptor);
    if(method == nullptr){
        method = lookupMethodInInterfaces(name, descriptor);
    }  
    return method; 
}

Method* Class::lookupMethodInClass(const string& name, const string& descriptor) const{
    for(const Class* c = this; c; c = superClass){
        for(Method* m : c->methods){
            if(m->name == name && m->descriptor == descriptor){
                return m;
            }
        }
    }
    return nullptr;
}

Method* Class::lookupMethodInInterfaces(const string& name, const string& descriptor) const{
    for(const Class* iface : interfaces){
        for(Method* m : iface->methods){
            if(m->name == name && m->descriptor == descriptor){
                return m;
            }
        }
        Method* method = iface->lookupMethodInInterfaces(name, descriptor);
        if(method){
            return method;
        }
    }
    return nullptr;
}

Method* Class::lookupInterfaceMethod(const string& name, const string& descriptor) const{
    for(Method* m : methods){
        if(m->name == name && m->descriptor == descriptor){
            return m;
        }
    }
    return lookupMethodInInterfaces(name, descriptor);
}

void Class::resolveSuperClass(){
    if(name == "java/lang/Object"){
        superClass = nullptr;
        return;
    }
    superClass = classLoader->loadClass(superClassName);
}

void Class::resolveInterfaces(){
    size_t interfaceCount = interfaceNames.size();
    interfaces = vector<Class*>(interfaceCount);
    for(size_t i = 0; i < interfaceCount; i++){
        interfaces[i] = classLoader->loadClass(interfaceNames[i]);
    }
}

void Class::verify(){
    // TODO
}

void Class::prepare(){
    calcInstanceFieldSlotIds();
    calcStaticFieldSlotIds();
    allocAndInitStaticVars();
}

void Class::link(){
    verify();
    prepare();
}

unsigned int Class::calcFieldSlotIds(bool staticOrNot){
    unsigned int slotId = 0;
    if(!staticOrNot && superClass != nullptr){
        slotId = superClass->instanceSlotCount;
    }
    for(Field* field : fields){
        bool flag = (field->isStatic() == staticOrNot);
        if(flag){
            field->slotId = slotId;
            slotId++;
            if(field->isLongOrDouble()){
                slotId++;
            }
        }
    }
    return slotId;
}
void Class::calcInstanceFieldSlotIds(){ instanceSlotCount = calcFieldSlotIds(false); }
void Class::calcStaticFieldSlotIds(){ staticSlotCount = calcFieldSlotIds(true); }

void Class::allocAndInitStaticVars(){
    staticVars = new Slots(staticSlotCount);
    for(Field* field : fields){
        if(field->isStatic() && field->isFinal()){
            initStaticFinalVar(field);
        }
    }
}

void Class::initStaticFinalVar(Field* field){
    Slots* vars = staticVars;
    ConstantPool* cp = constantPool;
    
    unsigned int cpIndex = field->constValueIndex;
    unsigned int slotId = field->slotId;
    if(cpIndex > 0){
        string& d = field->descriptor;
        if(d == "Z" || d == "B" || d == "C" || d == "S" || d == "I"){
            int val = cp->getConstant(cpIndex).getVal<int>();
            vars->set(slotId, val);
        }
        else if(d == "J"){
            long val = cp->getConstant(cpIndex).getVal<long>();
            vars->set(slotId, val);
        }
        else if(d == "F"){
            float val = cp->getConstant(cpIndex).getVal<float>();
            vars->set(slotId, val);
        }
        else if(d == "D"){
            double val = cp->getConstant(cpIndex).getVal<double>();
            vars->set(slotId, val);
        }
        else if(d == "Ljava/lang/String;"){
            // TODO
            cerr << "String TO DO" << endl;
        }
    }
}