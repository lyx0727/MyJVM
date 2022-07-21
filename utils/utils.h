#ifndef UTILS_GUARD
#define UTILS_GUARD
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
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

#endif