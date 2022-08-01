#include "sym_ref.h"
using namespace std;

Class* SymRef::resolvedClass() {
    if(_class == nullptr){
        try{
            resolveClassRef();
        }catch(...){
            throw;
        }
    }
    return _class;
}
void SymRef::resolveClassRef() {
    Class* d = cp->_class;
    Class* c = d->classLoader->loadClass(className);
    if(!c->isAccessibleTo(d)){
        throw JavaLangIllegalAccessError(className, __FILE__, __LINE__);
    }
    _class = c;
}

Field* FieldRef::resolvedField(){
    if(field == nullptr){
        try{
            resolveFieldRef();
        }catch(...){
            throw;
        }
    }
    return field;
}
void FieldRef::resolveFieldRef() {
    Class* d = cp->_class;
    Class* c = resolvedClass();
    field = c->lookupField(name, descriptor);
    if(field == nullptr){
        throw JavaLangClassNoSuchFieldError(name, __FILE__, __LINE__);
    }
    if(!field->isAccessibleTo(d)){
        throw JavaLangIllegalAccessError(name, __FILE__, __LINE__);
    }
}

Method* MethodRef::resolvedMethod(){
    if(method == nullptr){
        try{
            resolveMethodRef();
        }catch(...){
            throw;
        }
    }
    return method;
}
void MethodRef::resolveMethodRef() {
    Class* d = cp->_class;
    Class* c = resolvedClass();
    if(c->isInterface()){
        throw JavaLangIncompatibleClassChangeError(c->name + " is interface", __FILE__, __LINE__);
    }
    method = c->lookupMethod(name, descriptor);
    if(method == nullptr){
        throw JavaLangNoSuchMethodError(name, __FILE__, __LINE__);
    }
    if(!method->isAccessibleTo(d)){
        throw JavaLangIllegalAccessError(name, __FILE__, __LINE__);
    }
}

Method* InterfaceMethodRef::resolvedInterfaceMethod(){
    if(method == nullptr){
        resolveInterfaceMethodRef();
    }
    return method;
}
void InterfaceMethodRef::resolveInterfaceMethodRef(){
    Class* d = cp->_class;
    Class* c = resolvedClass();
    if(!c->isInterface()){
        throw JavaLangIncompatibleClassChangeError(c->name + " is not interface", __FILE__, __LINE__);
    }
    method = c->lookupInterfaceMethod(name, descriptor);
    if(method == nullptr){
        throw JavaLangNoSuchMethodError(name, __FILE__, __LINE__);
    }
    if(!method->isAccessibleTo(d)){
        throw JavaLangIllegalAccessError(name, __FILE__, __LINE__);
    }
}