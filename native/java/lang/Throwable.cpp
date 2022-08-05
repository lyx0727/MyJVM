#include "Throwable.h"
using namespace std;

StackTraceElement::StackTraceElement(Frame* frame){
    Method* method = frame->method;
    Class* _class = method->_class;
    fileName = _class->sourceFile;
    className = _class->name;
    methodName = method->name;
    lineNumber = method->getLineNumber(frame->nextPc - 1);
}

const string StackTraceElement::toString() const{
    return className + "." + methodName + "(" + fileName + ":" + to_string(lineNumber) +  ")";
}

StackTraceElements::StackTraceElements(Object* tObj, Thread* thread){
    // skip:
    //  fillInStackTrace(int)
    //  fillInStackTrace()
    //  <clint> of exception class
    int skip = tObj->_class->distanceToObject() + 2;
    vector<Frame*> frames = thread->getFrames();
    for(unsigned int i = skip; i < frames.size(); i++){
        stes.push_back(StackTraceElement(frames[i]));
    }
}

namespace java_lang_Throwable{
    // private native Throwable fillInStackTrace(int dummy);
    void fillInStackTrace(Frame* frame){
        Ref ref = frame->get<Ref>(0);
        Object* _this = (Object*)ref;
        frame->push(_this);
        StackTraceElements* stes = new StackTraceElements(_this, frame->thread);
        _this->setExtra(stes);
    }

    void init(){
        registerNative("java/lang/Throwable", "fillInStackTrace", "(I)Ljava/lang/Throwable;", fillInStackTrace);
    }
}