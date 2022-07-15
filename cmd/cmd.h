#ifndef CMD_GUARD
#define CMD_GUARD
#include <vector>
#include <string>

struct Cmd
{
    bool helpFlag = false;
    bool versionFlag = false;
    std::string cpOption;
    std::string Class;
    std::string XjreOption;
    std::vector<std::string> args;
};

Cmd parseCmd(int argc, char* argv[]);
void printUsage(char* argv[]);
#endif

