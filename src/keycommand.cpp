#include "keycommand.h"
#include <iostream>

/*
 * key 命令初始化函数
 * key command init function
 */
void KeyCmdInit(std::set<std::string>* pkeys_set)
{
    std::shared_ptr<std::set<std::string>> keys_shared(pkeys_set);
    CmdFactory& cmd_factory = CmdFactory::GetInstance();

    cmd_factory.RegisterCmdStrategy("DEL", new DelKeyCmd(keys_shared));
    cmd_factory.RegisterCmdStrategy("REGISTERKEY", new RegisterKeyCmd(keys_shared));
}

bool KeyCmd::IsExist(const std::string& key)
{
    return keys_->find(key) != keys_->end();
}

/*
 * DEL <key>
 * 
 * key DEL命令执行过程
 * key DEL command execution process
 */
void DelKeyCmd::Execute(const std::vector<std::string>& cmd, std::string& result)
{
    if(!IsExist(cmd[1]))
    {
        result.assign(RES_MSG[RES_NO_KEY]);
    }
    else
    {
        CmdStrategy* strategy = CmdFactory::GetInstance().GetCmdStrategy("DELSTRING");
        strategy->Execute(cmd, result);
    }
}

void RegisterKeyCmd::Execute(const std::vector<std::string>& cmd, std::string& result)
{
    if(IsExist(cmd[1]))
    {
        result.assign("[ERROR]: key is Exist\r\n");
    }
    else
    {
        keys_->insert(cmd[1]);
        result.assign("(integer) 1\r\n");
    }
}
