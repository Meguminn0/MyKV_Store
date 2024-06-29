#include "cmdfactory.h"
#include <string>

#include <iostream>

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
    virtual std::string Execute(const std::vector<std::string>& cmd)
    {
        return RES_MSG[RES_ERROR];
    }

    void Show() { std::cout << "ErrorStrategy SHOW" << std::endl;}
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