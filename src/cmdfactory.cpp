#include "cmdfactory.h"
#include <string>

const char* RES_MSG[] = {
    "OK\r\n", 
    "Already have this key!\r\n", 
    "Fail\r\n",
    "No such key!\r\n",
    "True\r\n", 
    "False\r\n",
    "Error command!\r\n"
};

/* ----------------------Error command------------------------- */
class ErrorStrategy : public CmdStrategy
{
public:
    void Execute(const std::vector<std::string>& cmd, std::string& result)
    {
        std::string str = "ERR unknown command ";
        str.append("\'" + cmd[0] + "\'" + ", with args beginning with: ");
        for(int i = 1; i < cmd.size(); ++i)
        {
            str.append("\'" + cmd[i] + "\' ");
        }
        str.append("\r\n");

        result.assign(str);
    }
};

CmdFactory& CmdFactory::GetInstance()
{
    static CmdFactory insatnce;
    return insatnce;
}

CmdStrategy* CmdFactory::GetCmdStrategy(const std::string& cmd_head)
{
    auto it = strategy_map_.find(cmd_head);
    if(it != strategy_map_.end())
    {
        return it->second;
    }
    else 
    {
        return strategy_map_["ERROR"];
    }
}

bool CmdFactory::RegisterCmdStrategy(const std::string& cmd_name, CmdStrategy* cmd_strategy)
{
    strategy_map_[cmd_name] = cmd_strategy;
    return true;
}

CmdFactory::CmdFactory()
{
    RegisterCmdStrategy("ERROR", new ErrorStrategy());
}

CmdFactory::~CmdFactory()
{
    for (auto& item : strategy_map_)
    {
        delete item.second;
        item.second = nullptr;
    }

    strategy_map_.clear();
}