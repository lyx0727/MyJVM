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
    std::vector<std::string> args;
};

Cmd parseCmd(int argc, char* argv[]);
void printUsage(Cmd cmd);
#endif

