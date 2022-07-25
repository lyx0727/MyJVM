#include "jvm_stack.h"
#include <iostream>
using namespace std;

void deleteFrame(Frame* frame){
    if(!frame) return;
    if(frame->lower){
        deleteFrame(frame->lower);
    }
    delete frame;
}

Stack::~Stack(){
    deleteFrame(_top);
}

void Stack::push(Frame* frame){
    if(sz >= maxSize){
        std::cerr << "java.lang.StackOverflowError" << std::endl;
        exit(1);
    }
    if(_top != nullptr){
        frame->lower = _top;
    }
    _top = frame;
    sz++;
}
Frame* Stack::top(){ 
    if(_top == nullptr){
        std::cerr << "jvm stack is empty!" << std::endl;
        exit(1);
    }
    return _top; 
}
Frame* Stack::pop(){
    if(_top == nullptr){
        std::cerr << "jvm stack is empty!" << std::endl;
        exit(1);
    }
    Frame* top = _top;
    _top = top->lower;
    top->lower = nullptr;
    sz--;
    return top;
}

const string OperandStack::toString() const{
    stack<Slot> st(slots);
    string str;
    while(!st.empty()){
        str += "(" + to_string((unsigned long)st.top().ref) +")\n";
        st.pop();
    }
    return str;
}
