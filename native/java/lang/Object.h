#ifndef JAVA_LANG_OBJECT_GUARD
#define JAVA_LANG_OBJECT_GUARD
#include "../../registry.h"

namespace java_lang_Object{
    // public final native Class<?> getClass();
    void getClass(Frame* frame);

    void init();
}

#endif