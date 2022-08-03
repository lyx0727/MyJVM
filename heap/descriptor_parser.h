#ifndef DESCRIPTOR_PARSER_GUARD
#define DESCRIPTOR_PARSER_GUARD
#include <string>
#include <vector>

struct MethodDescriptor{
    std::vector<std::string> parameterTypes;
    std::string              returnType;
    MethodDescriptor(){}
    MethodDescriptor(const std::vector<std::string>& parameterTypes, const std::string &returnType)
        :parameterTypes(parameterTypes), returnType(returnType){}
};

struct MethodDescriptorParser{
    size_t cur;
    std::string d;
    MethodDescriptorParser(const std::string& descriptor): cur(0), d(descriptor){}

    MethodDescriptor         parse();

    std::vector<std::string> parseParamTypes();
    std::string              parseReturnType();

    std::string              parseFieldType();
    std::string              parseObjectType();
    std::string              parseArrayType();
};

#endif