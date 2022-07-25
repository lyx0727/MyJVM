#include "descriptor_parser.h"
#include <iostream>
using namespace std;

// ???
MethodDescriptor MethodDescriptorParser::parse(){
    auto paramTypes = parseParamTypes();
    auto returnType = parseReturnType();
    return MethodDescriptor(
        paramTypes,
        returnType
    );
}

string MethodDescriptorParser::parseReturnType(){
    string type = parseFieldType();
    if(type == ""){
        if(d[cur] == 'V'){
            type = "V";
        }
    }
    return type;
}
vector<string> MethodDescriptorParser::parseParamTypes(){
    vector<string> types;
    if(cur >= d.length() || d[cur] != '('){
        return types;
    }
    cur++;
    string type = "";
    while(1){
         type = parseFieldType();
         if(type == "") break;
         types.push_back(type);
    }
    if(cur >= d.length() || d[cur] != ')'){
        types.clear();
    }
    cur++;
    return types;
}
string MethodDescriptorParser::parseFieldType(){
    if(cur < d.length()){
        switch(d[cur]){
            case 'B': case 'C': case 'D': case 'F': case 'I': case 'J': case 'S': case 'Z':
                return string(1, d[cur++]);
            case 'L':
                return parseObjectType();
            case '[':
                return parseArrayType();
        }
    }
    return "";
}
string MethodDescriptorParser::parseObjectType(){
    for(size_t i = cur; i < d.length(); i++){
        if(d[i] == ';'){
            string type = d.substr(cur, i - cur + 1);
            cur = i + 1;
            return type;
        }
    }
    return "";
}
string MethodDescriptorParser::parseArrayType(){
    cur++;
    string type = parseFieldType();
    return "[" + type; 
}