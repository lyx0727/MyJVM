#ifndef CLASSPATH_GUARD
#define CLASSPATH_GUARD
#include <string>
#include "entry.h"

class Classpath{
private:
    Entry* bootClasspath;
    Entry* extClasspath;
    Entry* userClasspath;
public:
    void parseBootAndExtClasspath(const std::string& jreOption);
    void parseUserClasspath(const std::string& cpOption);
    std::vector<Byte> readClass(const std::string& className);
};

Classpath* parse(const std::string& jreOption, const std::string& cpOption);

const std::string getJreDir(const std::string& jreOption);

#endif