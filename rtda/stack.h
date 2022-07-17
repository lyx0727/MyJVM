#ifndef JVM_STACK_GUARD
#define JVM_STACK_GUARD
#include <iostream>
#include "frame.h"

class Stack{
private:
    unsigned int maxSize;
    unsigned int size;
    Frame* _top;
public:
    Stack(unsigned int maxSize): maxSize(maxSize){}
    void push(Frame* frame){
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
    Frame* top(){ 
        if(_top == nullptr){
            std::cerr << "jvm stack is empty!" << std::endl;
            exit(1);
        }
        return _top; 
    }
    Frame* pop(){
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
};

#endif