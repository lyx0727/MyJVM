#ifndef UTILS_GUARD
#define UTILS_GUARD
#include <cstring>
#include <string>
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

template<typename T>int toInt(T val){
    int i;
    std::memcpy(&i, &val, sizeof(int) / sizeof(Byte));
    return i;
}
template<typename T>float toFloat(T val){
    float f;
    std::memcpy(&f, &val, sizeof(float) / sizeof(Byte));
    return f;
}
template<typename T>long toLong(T val){
    long l;
    std::memcpy(&l, &val, sizeof(long) / sizeof(Byte));
    return l;
}
template<>long toLong(int val);
template<typename T>double toDouble(T val){
    double db;
    std::memcpy(&db, &val, sizeof(double) / sizeof(Byte));
    return db;
}

template<typename T>
void print(const std::vector<T>& vec){
    for(const T& x : vec){
        std::cout << std::hex << (int)x << ' ';
    }
    std::cout << std::endl;
}

#endif