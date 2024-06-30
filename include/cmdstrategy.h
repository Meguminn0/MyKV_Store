#ifndef CMD_STRATEGY_H_
#define CMD_STRATEGY_H_
#include <string>
#include <vector>

class CmdStrategy
{
public:
    virtual void Execute(const std::vector<std::string>& cmd, std::string& result) = 0;
    virtual ~CmdStrategy() { }
};

#endif