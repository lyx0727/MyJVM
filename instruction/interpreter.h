#ifndef INTERPRETER_GUARD
#define INTERPRETER_GUARD
#include <vector>
#include "../classfile/member_info.h"
#include "../rtda/frame.h"

void catchErr(Frame* frame);

void loop(Thread& thread, const std::vector<Byte>& code);

void interpret(classfile::MemberInfo* memberInfo);

void interpret(Method* method);

#endif