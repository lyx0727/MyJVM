#ifndef REGISTRY_GUARD
#define REGISTRY_GUARD
#include <map>
#include <string>
#include "../utils/utils.h"
#include "../rtda/frame.h"

struct Frame;

typedef void (*NativeMethod)(Frame*);

std::map<std::string, NativeMethod> nativeRegistry;

void emptyNativeMethod(Frame* frame);

const std::string getNativeKey(const std::string& className, const std::string& methodName, const std::string& methodDescriptor);

void registerNative(const std::string& className, const std::string& methodName, const std::string& methodDescriptor, NativeMethod method);
NativeMethod findNative(const std::string& className, const std::string& methodName, const std::string& methodDescriptor);
#endif