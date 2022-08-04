#ifndef JAVA_LANG_THROWABLE_GUARD
#define JAVA_LANG_THROWABLE_GUARD
#include "../../registry.h"

namespace java_lang_Throwable{
    // private native Throwable fillInStackTrace(int dummy);
    void fillInStackTrace(Frame* frame){
    
    }

    void init(){
        registerNative("java/lang/Throwable", "fillInStackTrace", "(I)Ljava/lang/Throwable;", fillInStackTrace);
    }
}

#endif