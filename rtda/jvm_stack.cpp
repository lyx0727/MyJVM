#include "jvm_stack.h"
#include <iostream>
using namespace std;

void deleteFrame(Frame* frame){
    if(frame->lower){
        deleteFrame(frame->lower);
    }
    delete frame;
}

Stack::~Stack(){
    deleteFrame(_top);
}

void Stack::push(Frame* frame){
    if(size >= maxSize){
        std::cerr << "java.lang.StackOverflowError" << std::endl;
        exit(1);
    }
    if(_top != nullptr){
        frame->lower = _top;
    }
    _top = frame;
    size++;
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
    size--;
    return top;
}
