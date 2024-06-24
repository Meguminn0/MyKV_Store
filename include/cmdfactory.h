#ifndef CMD_FACTORY_H_
#define CMD_FACTORY_H_

#include "cmdstrategy.h"
#include <string>
#include <map>

class CmdFactory
{
public:
    static CmdFactory& GetInstance();

    CmdStrategy* GetCmdStrategy(const std::string& cmd_head);
    bool RegisterCmdStrategy(const std::string& cmd_name, CmdStrategy* cmd_strategy);

protected:
    CmdFactory();
    ~CmdFactory();
    CmdFactory(const CmdFactory& cmdfactory) = delete;
    CmdFactory& operator=(const CmdFactory& cmdfactory) = delete;

private:
    std::map<std::string, CmdStrategy*> strategy_map_;
};

#endif