#include "class.h"
#include "sym_ref.h"
using namespace std;

Class::Class()
: constantPool(nullptr), classLoader(nullptr), superClass(nullptr), staticVars(nullptr), initStarted(false), jClass(nullptr){}

Class::Class(classfile::Classfile& cf)
: classLoader(nullptr), superClass(nullptr), initStarted(false), jClass(nullptr){
    accessFlag = cf.getAccessFlags();
    name = cf.getClassName();
    constantPool = new ConstantPool(this, cf.getConstantPool());
    superClassName = cf.getSuperClassName();
    interfaceNames = cf.getInterfacesNames();
    getFields(cf.getFields());
    getMethods(cf.getMethods());
    sourceFile = cf.getSourceFile();
}

Class::~Class(){ 
    if(constantPool){
        delete constantPool;
        constantPool = nullptr;
    } 
    if(staticVars){
        delete staticVars;
        staticVars = nullptr;
    }
}

Object* Class::newArray(unsigned int count) {
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
Object* Class::newMultiDimensionalArray(int* counts, int dimensions){
    unsigned int count = (unsigned int)counts[0];
    Object* arr = newArray(count);
    if(dimensions > 1){
        Object** refs = arr->getRefs();
        Class* componentClass = getComponentClass();
        for(unsigned int i = 0; i < count; i++){
            refs[i] = componentClass->newMultiDimensionalArray(counts + 1, dimensions - 1);
        }
    }
    return arr;
}
Object* Class::newObject(){ return new Object(this); }

Class* Class::getArrayClass() const { return classLoader->loadClass("[" + toDescriptor(name) ); }
Class* Class::getComponentClass() const {  
    if(name[0] == '['){
        return classLoader->loadClass(toClassName(name.substr(1)));
    }
    cerr << "Not array: " << name << endl;
    exit(1);
    return nullptr;
}

const string toDescriptor(const string& className){
    if(className[0] == '[') return className;
    if(primitiveTypeMap.count(className)){
        return primitiveTypeMap.at(className);
    }
    return "L" + className + ";";
}
const std::string toClassName(const std::string& descriptor){
    if(descriptor[0] == '[') return descriptor;
    if(descriptor[0] == 'L') return descriptor.substr(1, descriptor.length() - 2);
    for(auto& it : primitiveTypeMap){
        if(descriptor == it.second){
            return it.first;
        }
    }
    cerr << "Invalid descriptor: " << descriptor << endl;
    exit(1);
    return "";
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
    for(const Class* t = superClass; t; t = t->superClass){
        if(t == c) 
            return true;
    }
    return false;
}
bool Class::isSuperClassOf(const Class* c) const{ return c->isSubClassOf(this); }

bool Class::isSubInterfaceOf(const Class* iface) const {
    for(const Class* const i : interfaces){
        if(i == iface || i->isSubInterfaceOf(iface))
            return true;
    }
    return false;
}
bool Class::isSuperInterfaceOf(const Class* iface) const { return iface->isSubInterfaceOf(this); }

bool Class::isImplements(const Class* iface) const {
    for(const Class* t = this; t; t = t->superClass){
        for(Class* i : t->interfaces){
            if(i == iface || i->isSubInterfaceOf(iface))
                return true;
        }
    }
    return false;
}

bool Class::isAssignableFrom(const Class* other) const {
    const Class* s = other;
    const Class* t = this;
    if(s == t) return true;
    if(!s->isArray()){
        if(!s->isInterface()){
            if(!t->isInterface()){
                // both s and t are normal class
                return s->isSubClassOf(t);
            } else{
                // s is class, t is interface
                return s->isImplements(t);
            }
        } else{
            // s is interface
            if(!t->isInterface()){
                return t->name == "java/lang/Object";
            }else{
                return t->isSuperInterfaceOf(s);
            }
        }
    }else{
        // s is array
        if(!t->isArray()){
            if(!t->isInterface()){
                // array extends Object 
                return t->name == "java/lang/Object";
            }else{
                // array implements Cloneable and Serializable
                return t->name == "java/lang/Cloneable" || t->name == "java/io/Serializable";
            }
        }else{
            // both s and t are array
            Class* sc = s->getComponentClass();
            Class* tc = t->getComponentClass();
            return sc == tc || tc->isAssignableFrom(sc);
        }
    }
}

Field* Class::getField(const string& name, const string& descriptor, bool isStatic) const{
    for(const Class* c = this; c; c = c->superClass){
        for(Field* field : c->fields){
            if(field->isStatic() == isStatic && field->name == name && field->descriptor == descriptor){
                return field;
            }
        }
    }
    return nullptr;
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
    for(const Class* c = this; c; c = c->superClass){
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

unsigned int Class::calcFieldSlotIds(bool isStatic){
    unsigned int slotId = 0;
    if(!isStatic && superClass != nullptr){
        slotId = superClass->instanceSlotCount;
    }
    for(Field* field : fields){
        bool flag = (field->isStatic() == isStatic);
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
            string str = cp->getConstant(cpIndex).getVal<string>();
            vars->set(slotId, classLoader->JString(str));
        }
    }
}

int Class::distanceToObject() const{
    int dist = 0;
    for(const Class* c = superClass; c; c = c->superClass){
        dist++;
    }
    return dist;
}