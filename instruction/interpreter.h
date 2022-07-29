#ifndef INTERPRETER_GUARD
#define INTERPRETER_GUARD
#include <vector>
#include "../classfile/member_info.h"
#include "../rtda/frame.h"

void logFrames(Thread& thread);
void logInstruction(Frame* frame, uint8_t opCode);

void catchErr(Frame* frame);

void loop(Thread& thread, bool logInst = false);

void interpret(Method* method, bool logInst = false);

// for low version
void interpret(classfile::MemberInfo* memberInfo, bool logInst = false);

#endif