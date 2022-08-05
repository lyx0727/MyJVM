#include "thread.h"
#include "../native/java/lang/Throwable.h"
using namespace std;
using namespace java_lang_Throwable;

const string Slots::toString() const {
    string str;
    for(Slot var : slots){
        str += "(" + to_string((unsigned long)(var.ref)) + ")\n";
    }
    return str;
}   

Thread::Thread(unsigned int maxSize):stack(new Stack(maxSize)){}
Thread::~Thread(){ delete stack; }

unsigned int Thread::size() const {
    return stack->size();
}

bool Thread::empty() const { return size() == 0; }

void Thread::clear(){ stack->clear(); }

Frame* Thread::newFrame(Method* method){ return new Frame(this, method); }
void Thread::pushFrame(Frame* frame){ stack->push(frame); }   
Frame* Thread::popFrame(){ return stack->pop(); }
Frame* Thread::getCurrentFrame(){ return stack->top(); }

vector<Frame*> Thread::getFrames() const{ return stack->getFrames(); }

bool Thread::findAndGotoExceptionHandler(Object* ex){
    while(!empty()){
        Frame* frame = getCurrentFrame();
        int pc = frame->nextPc - 1;
        int handlerPc = frame->method->findExceptionHandler(ex->_class, pc);
        if(handlerPc > 0){
            frame->operandStack.clear();
            frame->push(ex);
            frame->nextPc = handlerPc;
            return true;
        }
        popFrame();
    }
    return false;
}
void Thread::handleUncaughtException(Object* ex){
    clear();
    Object* jMsg = ex->getRefVar("detailMessage", "Ljava/lang/String;");
    string cMsg = jMsg->CString();
    cout << ex->_class->getJavaName() << ": " << cMsg << endl;
    StackTraceElements* stes = (StackTraceElements*)(ex->extra);
    for(unsigned int i = 0; i < stes->size(); i++){
        cout << "\tat " << stes->get(i).toString(); 
    }
}