#include "cmdfactory.h"
#include <string>

// 给客户端返回信息的枚举
enum result_t{
    // set、delete
    KV_RES_OK = 0,
    KV_RES_AL_HAVE,
    KV_RES_FAIL,
    // get、delete
    KV_RES_NO_KEY,
    // exist
    KV_RES_TRUE,
    KV_RES_FALSE,
    // other
    KV_RES_ERROR,
};

// 列出具体的返回信息
const char* RES_MSG[] = {
    "OK\r\n", "Already have this key!\r\n", "Fail\r\n",
    "No such key!\r\n",
    "True\r\n", "False\r\n",
    "Error command!\r\n"
};

class ErrorStrategy : public CmdStrategy
{
public:
    virtual std::string Execute(const std::vector<std::string>& cmd)
    {
        return RES_MSG[KV_RES_ERROR];
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
    if(!strategy_map_.empty())
    {
        for(auto item : strategy_map_)
        {
            delete item.second;
            item.second = nullptr;
        }

        strategy_map_.clear();
    }
}