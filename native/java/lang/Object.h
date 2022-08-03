#ifndef JAVA_LANG_OBJECT_GUARD
#define JAVA_LANG_OBJECT_GUARD
#include "../../registry.h"

namespace java_lang_Object{
    // public final native Class<?> getClass();
    inline void getClass(Frame* frame){
        Ref ref = frame->get<Ref>(0);
        Object* _this = (Object*)ref;
        Object* _class = _this->_class->jClass;
        frame->push(_class);
    }

    inline void init(){
        registerNative("java/lang/Object", "getClass", "()Ljava/lang/Class;", getClass);
    }
}

#endif