#include "utils.h"
#include <dirent.h>
#include <cstring>
#include <codecvt>
#include <locale> 
#include <iostream>
#include <unistd.h>
using namespace std;

const string getSuffix(const string& s, size_t n){
    if(n >= s.length()){
        return s;
    }
    return s.substr(s.length() - n);
}

const string replace(const string& s, char oldVal, char newVal){
    string res;
    for(char c : s){
        if(c == oldVal){
            res += newVal;
        }
        else{
            res += c;
        }
    }
    return res;
}

const u16string utf8_to_utf16(const string& str){
    return wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(str);
}
const string utf16_to_utf8(const u16string& str){
    return wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(str);
}

bool isDir(const string& path){
    return opendir(path.c_str()) != nullptr;
}

bool isZip(const string& path){
    string suffix = getSuffix(path, 4);
    return suffix == ".jar" || suffix == ".zip" || suffix == ".JAR" || suffix == ".ZIP";
}

bool isAbsPath(const string& path){
    return path[0] == '/';
}

const string getAbsPath(const string& path){
    if(isAbsPath(path)){
        return path;
    }
    const int maxSize = 256;
    char* buf = new char[maxSize];
    string absPath =  getcwd(buf, 256);
    delete[] buf;
    return absPath + "/" + path;
}

void getFileNames(const string& path, vector<string>& fileNames){
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(path.c_str()))){
        cerr << "Folder " << path << " doesn't exist!" << endl;
        return;
    }
    while((ptr = readdir(pDir))!=0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0){
            fileNames.push_back(path + "/" + ptr->d_name);
        }
    }
    closedir(pDir);
}

void walkDir(const std::string& path, WalkFn walkFn, void* that = nullptr){
    vector<string> fileNames;
    getFileNames(path, fileNames);
    for(const string& fileName : fileNames){
        walkFn(fileName, that);
    }
}