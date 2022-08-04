#ifndef JAVA_LANG_THROWABLE_GUARD
#define JAVA_LANG_THROWABLE_GUARD
#include "../../registry.h"

struct Object;
struct Thread;

namespace java_lang_Throwable{
    struct StackTraceElement{
        std::string fileName;
        std::string className;
        std::string methodName;
        int         lineNumber;
        StackTraceElement(Frame* frame);
        const std::string toString() const;
    };

    struct StackTraceElements{
    private:
        std::vector<StackTraceElement> stes;
    public:
        StackTraceElements(Object* tObj, Thread* thread);
    
        const StackTraceElement& get(size_t i) const { return stes[i]; }
        size_t size() const { return stes.size(); }
    };

    // private native Throwable fillInStackTrace(int dummy);
    void fillInStackTrace(Frame* frame);

    void init();
}

#endif