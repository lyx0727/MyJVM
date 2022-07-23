#include "class_member.h"
using namespace std;

vector<Field*> getFields(Class* _class, const vector<MemberInfo*>& cfFields){
    vector<Field*> fields(cfFields.size());
    for(size_t i = 0; i < cfFields.size(); i++){
        fields[i] = new Field(_class, cfFields[i]);
    }
    return fields;
}

vector<Method*> getFethods(Class* _class, const std::vector<MemberInfo*>& cfMethods){
    vector<Method*> methods(cfMethods.size());
    for(size_t i = 0; i < cfMethods.size(); i++){
        MemberInfo* method = cfMethods[i];
        methods[i] = new Method(_class, method);
        CodeAttribute* codeAttr = method->getCodeAttribute();
        methods[i]->code = codeAttr->code;
        methods[i]->maxLocals = codeAttr->maxLocals;
        methods[i]->maxStack = codeAttr->maxStack;
    }
    return methods;
}