#ifndef CMD_FACTORY_H_
#define CMD_FACTORY_H_

#include "cmdstrategy.h"
#include <string>
#include <map>

// 给客户端返回信息的枚举
enum result_t{
    // set、delete
    RES_OK = 0,
    RES_AL_HAVE,
    RES_FAIL,
    // get、delete
    RES_NO_KEY,
    // exist
    RES_TRUE,
    RES_FALSE,
    // error
    RES_ERROR,
};

// 列出具体的返回信息
extern const char* RES_MSG[];

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