#include "class_member.h"
using namespace std;

bool ClassMember::isAccessibleTo(Class* d) const{
    if(isPublic()) return true;
    Class* c = _class;
    if(isProtected()){
        return d == c || d->isSubClassOf(c) || c->getPackageName() == d->getPackageName();
    }
    if(!isPrivate()){
        return c->getPackageName() == d->getPackageName();
    }
    return d == c;
}

uint32_t Method::calcArgSlotCount(){
    argSlotCount = 0;
    for(const string& paramterType : md.parameterTypes){
        argSlotCount++;
        if(paramterType == "J" || paramterType == "D"){
            argSlotCount++;
        }
    }
    // non static method also has 'this'
    if(!isStatic()){
        argSlotCount++;
    }
    return argSlotCount;
}

void Method::injectCodeAttribute(){
    maxStack = 4;
    maxLocals = argSlotCount;
    switch(md.returnType[0]){
        // 0xfe: INVOKE_NATIVE
        case 'V': code = vector<Byte>{ 0xfe, 0xb1 }; break;   // RETURN
        case 'D': code = vector<Byte>{ 0xfe, 0xaf }; break;   // DRETURN
        case 'F': code = vector<Byte>{ 0xfe, 0xae }; break;   // FRETURN
        case 'J': code = vector<Byte>{ 0xfe, 0xad }; break;   // LRETURN
        case 'L': 
        case '[': code = vector<Byte>{ 0xfe, 0xb0 }; break;   // ARETURN
        default:  code = vector<Byte>{ 0xfe, 0xac }; break;   // IRETUEN
    }
}

bool ClassMember::isStatic() const { return ::isStatic(accessFlag); }
bool ClassMember::isFinal() const { return ::isFinal(accessFlag); }
bool ClassMember::isAbstract() const { return ::isAbstract(accessFlag); }
bool ClassMember::isNative() const { return ::isNative(accessFlag); }

bool ClassMember::isPublic() const { return ::isPublic(accessFlag); }
bool ClassMember::isProtected() const { return ::isProtected(accessFlag); }
bool ClassMember::isPrivate() const { return ::isPrivate(accessFlag); }
bool ClassMember::isLongOrDouble() const { return descriptor == "J" || descriptor == "D"; }