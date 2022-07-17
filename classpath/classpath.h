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
    Classpath(const std::string& jreOption, const std::string& cpOption){
        parse(jreOption, cpOption);
    }
    ~Classpath(){
        delete bootClasspath;
        delete extClasspath;
        delete userClasspath;
    }
    
    void parse(const std::string& jreOption, const std::string& cpOption){
        parseBootAndExtClasspath(jreOption);
        parseUserClasspath(cpOption);
    }
    void parseBootAndExtClasspath(const std::string& jreOption);
    void parseUserClasspath(const std::string& cpOption);
    
    std::vector<Byte> readClass(const std::string& className);

    const std::string toString() const {
        return "----------[boot]----------\n" + bootClasspath->toString()
            + "\n----------[ext]----------\n" + extClasspath->toString()
            + "\n----------[user]----------\n" + userClasspath->toString();
    }
};

const std::string getJreDir(const std::string& jreOption);

#endif