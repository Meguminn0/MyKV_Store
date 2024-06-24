#ifndef CMD_STRATEGY_H_
#define CMD_STRATEGY_H_
#include <string>
#include <vector>

class CmdStrategy
{
public:
    virtual std::string Execute(const std::vector<std::string>& cmd) = 0;
    virtual ~CmdStrategy() { }
};

#endif