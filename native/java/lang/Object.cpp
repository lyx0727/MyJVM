#include "Object.h"
using namespace std;

namespace java_lang_Object{
    void getClass(Frame* frame){
        Ref ref = frame->get<Ref>(0);
        Object* _this = (Object*)ref;
        Object* _class = _this->_class->jClass;
        frame->push(_class);
    }

    void init(){
        registerNative("java/lang/Object", "getClass", "()Ljava/lang/Class;", getClass);
    }
}