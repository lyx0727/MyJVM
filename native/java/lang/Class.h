#ifndef JAVA_LANG_CLASS_GUARD
#define JAVA_LANG_CLASS_GUARD
#include "../../registry.h"

namespace java_lang_Class{
    // static native Class<?> getPrimitiveClass(String name);
    void getPrimitiveClass(Frame* frame){
        Ref ref = frame->get<Ref>(0);
        Object* nameObj = (Object*)ref;
        std::string name = nameObj->CString();
        ClassLoader* loader = frame->getClassLoader();
        Object* _class = loader->loadClass(name)->jClass;
        frame->push(_class);
    }

    // private native String getName0();
    void getName0(Frame* frame){
        Ref ref = frame->get<Ref>(0);
        Object* _this = (Object*)ref;
        Class* _class = (Class*)_this->extra;
        std::string name = _class->getJavaName();
        Object* nameObj = _class->classLoader->JString(name);
        frame->push(nameObj);
    }

    // private static native boolean desiredAssertionStatus0(Class<?> clazz);
    void desiredAssertionStatus0(Frame* frame){
        frame->push<Byte>(false);
    }

    inline void init(){
        registerNative("java/lang/Class", "getPrimitiveClass", "(Ljava/lang/String;)Ljava/lang/Class;", getPrimitiveClass);
        registerNative("java/lang/Class", "getName0", "()Ljava/lang/String;", getName0);
        registerNative("java/lang/Class", "desiredAssertionStatus0", "(Ljava/lang/Class;)Z", desiredAssertionStatus0);
    }
}

#endif