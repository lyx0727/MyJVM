#ifndef UTILS_GUARD
#define UTILS_GUARD
#include <cstring>
#include <string>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <exception>
#include <iostream>

typedef unsigned char Byte;
typedef void*         Ref;

const std::string getSuffix(const std::string& s, size_t n);
const std::string replace(const std::string& s, char oldVal, char newVal);

bool isDir(const std::string& path);
bool isZip(const std::string& path);
bool isAbsPath(const std::string& path);
const std::string getAbsPath(const std::string& path);

typedef void (*WalkFn)(const std::string&, void*);
void walkDir(const std::string& path, WalkFn walkFn, void* that);

void getFileNames(const std::string& path, std::vector<std::string>& fileNames);

template<typename Y, typename X> Y to(X x){
    unsigned int size = std::min(sizeof(X), sizeof(Y));
    Y y;
    std::memcpy(&y, &x, size / sizeof(Byte));
    return y;
}
template<> inline long to(int i){
    long l;
    std::memcpy(&l, &i, sizeof(int) / sizeof(Byte));
    // set 0 to high
    l &= 0x00000000ffffffffL;
    return l;
}
template<typename X> int toInt(X x){ return to<int>(x); }
template<typename X> int toLong(X x){ return to<long>(x); }
template<typename X> int toFloat(X x){ return to<float>(x); }
template<typename X> int toDouble(X x){ return to<double>(x); }

template<typename T>
void print(const std::vector<T>& vec){
    for(const T& x : vec){
        std::cout << std::hex << (int)x << ' ';
    }
    std::cout << std::endl;
}

// Exception
struct JavaLangException : public std::exception {
private:
    std::string type;
    std::string fileName;
    unsigned int lineNumber;
    char* msg;
public:
    JavaLangException(const std::string& type, const char* fileName, unsigned int lineNumber, const std::string& message)
        : type(type), fileName(fileName), lineNumber(lineNumber){
        std::string s = this->fileName + ":" + std::to_string(this->lineNumber) + ": java.lang." + type + ": " + message;
        int len = s.length();
        msg = new char[len + 1];
        std::strcpy(msg, s.c_str());
    }
    virtual ~JavaLangException(){ delete msg; }
    const char * what () const throw (){
        return msg;
    }
};

struct JavaLangClassNotFoundException : public JavaLangException {
    JavaLangClassNotFoundException(const std::string& className, const char* fileName, unsigned int lineNumber)
    : JavaLangException("ClassNotFoundException", fileName, lineNumber, className){}
};
struct JavaLangClassNoSuchFieldError : public JavaLangException {
    JavaLangClassNoSuchFieldError(const std::string& fieldName, const char* fileName, unsigned int lineNumber)
    : JavaLangException("ClassNoSuchFieldError", fileName, lineNumber, fieldName){}
};
struct JavaLangIllegalAccessError: public JavaLangException {
    JavaLangIllegalAccessError(const std::string& name, const char* fileName, unsigned int lineNumber)
    : JavaLangException("IllegalAccessError", fileName, lineNumber, name){}
};

#endif