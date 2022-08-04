#ifndef JAVA_LANG_CLASS_GUARD
#define JAVA_LANG_CLASS_GUARD
#include "../../registry.h"

namespace java_lang_Class{
    // static native Class<?> getPrimitiveClass(String name);
    void getPrimitiveClass(Frame* frame);

    // private native String getName0();
    void getName0(Frame* frame);

    // private static native boolean desiredAssertionStatus0(Class<?> clazz);
    void desiredAssertionStatus0(Frame* frame);

    void init();
}

#endif