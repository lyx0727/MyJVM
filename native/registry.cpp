#include "registry.h"
#include "java/lang/Class.h"
#include "java/lang/Object.h"
#include "java/lang/Throwable.h"
using namespace std;

map<string, NativeMethod> nativeRegistry = map<string, NativeMethod>();

void emptyNativeMethod(Frame* frame){
    // do nothing
}

const string getNativeKey(const string& className, const string& methodName, const string& methodDescriptor){
    return className + "~" + methodName + "~" + methodDescriptor;
}

void registerNative(const string& className, const string& methodName, const string& methodDescriptor, NativeMethod method){
    const string key = getNativeKey(className, methodName, methodDescriptor);
    nativeRegistry[key] = method;
}

NativeMethod findNative(const string& className, const string& methodName, const string& methodDescriptor){
    const string key = getNativeKey(className, methodName, methodDescriptor);
    if(nativeRegistry.count(key)){
        return nativeRegistry.at(key);
    }
    if(methodDescriptor == "()V" && methodName == "registerNatives"){
        return emptyNativeMethod;
    }
    return nullptr;
}

void init(){
    java_lang_Class::init();
    java_lang_Object::init();
    java_lang_Throwable::init();
}