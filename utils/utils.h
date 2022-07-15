#ifndef UTILS_GUARD
#define UTILS_GUARD
#include <string>
#include <vector>
#include <iostream>

typedef unsigned char Byte;

const std::string getSuffix(const std::string& s, size_t n);
const std::string replace(const std::string& s, char oldVal, char newVal);

bool isDir(const std::string& path);
bool isZip(const std::string& path);
bool isAbsPath(const std::string& path);
const std::string getAbsPath(const std::string& path);

typedef void (*WalkFn)(const std::string&, void*);
void walkDir(const std::string& path, WalkFn walkFn, void* that);

void getFileNames(const std::string& path, std::vector<std::string>& fileNames);

template<typename T>
void print(const std::vector<T>& vec){
    for(const T& x : vec){
        std::cout << std::hex << (int)x << ' ';
    }
    std::cout << std::endl;
}

#endif