#ifndef ENTRY_GUARD
#define ENTRY_GUARD
#include <vector>
#include <string>
#include <ostream>
#include "../utils/utils.h"

class Entry{
public:
    virtual std::vector<Byte> readClass(const std::string& className) = 0;
    virtual const std::string toString() const = 0; 
    virtual ~Entry(){};
};

/*
 * DirEntry
 */
class DirEntry : public Entry{
private:
    std::string absDir;
public:
    DirEntry(const std::string& path);
    std::vector<Byte> readClass(const std::string& className); 
    const std::string toString() const { return this->absDir; }
    ~DirEntry(){}
};

/*
 * ZipEntry
 */
class ZipEntry : public Entry{
private:
    std::string absDir;
public:
    ZipEntry(const std::string& path);
    std::vector<Byte> readClass(const std::string& className); 
    const std::string toString() const { return this->absDir; }
    ~ZipEntry(){}
};

/*
 * CompositeEntry
 */
class CompositeEntry : public Entry{
private:
    std::vector<Entry*> entrys;
public:
    CompositeEntry(const std::vector<std::string>& paths);
    std::vector<Byte> readClass(const std::string& className); 
    const std::string toString() const;
    ~CompositeEntry();
};

/*
 * WildcardEntry
 */
class WildcardEntry : public Entry{
private:
    std::vector<Entry*> entrys;
public:
    WildcardEntry(const std::string& path);
    void addEntry(Entry* entry){ entrys.push_back(entry); }
    std::vector<Byte> readClass(const std::string& className); 
    const std::string toString() const;
    ~WildcardEntry();
};

std::ostream& operator<<(std::ostream& os, const Entry& entry);

Entry* getEntry(const std::string& path);

#endif