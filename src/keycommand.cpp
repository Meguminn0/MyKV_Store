#include "keycommand.h"

/*
 * key 命令初始化函数
 * key command init function
 */
void KeyCmdInit(std::set<std::string>* pkeys_set)
{
    std::shared_ptr<std::set<std::string>> keys_shared(pkeys_set);
    CmdFactory& cmd_factory = CmdFactory::GetInstance();

    cmd_factory.RegisterCmdStrategy("DEL", new DelKeyCmd(keys_shared));
    cmd_factory.RegisterCmdStrategy("REGISTER_KEY", new RegisterKeyCmd(keys_shared));
    cmd_factory.RegisterCmdStrategy("DEREGISTER_KEY", new DeregisterKeyCmd(keys_shared));
    cmd_factory.RegisterCmdStrategy("KEYS", new KeysKeyCmd(keys_shared));
}

bool KeyCmd::IsExist(const std::string& key)
{
    return keys_->find(key) != keys_->end();
}

/*
 * DEL <key>
 * 
 * DEL命令执行过程
 * DEL command execution process
 */
void DelKeyCmd::Execute(const std::vector<std::string>& cmd, std::string& result)
{
    if(!IsExist(cmd[1]))
    {
        result.assign("(integer) 0");
    }
    else
    {
        CmdStrategy* strategy = CmdFactory::GetInstance().GetCmdStrategy("DEL_STRING");
        strategy->Execute(cmd, result);
        
        if(result == "(integer) 1\r\n")
        {
            strategy = CmdFactory::GetInstance().GetCmdStrategy("DEREGISTER_KEY");
            std::string res;
            strategy->Execute(cmd, res);
        }
    }
}

/*
 * KEYS <pattern>
 * 
 * KEYS命令执行过程
 * KEYS command execution process
 */
void KeysKeyCmd::Execute(const std::vector<std::string>& cmd, std::string& result)
{
    if(keys_->empty())
    {
        result.assign("(empty array)\r\n");
    }
    else if(cmd[1] != "*")
    {
        result.assign("ERR can not parse this pattern " + cmd[1]);
    }
    else
    {
        int sz = keys_->size();
        int index = 0;
        for(auto it = keys_->begin(); it != keys_->end(); ++it)
        {
            result.append(std::to_string(++index) + ") \"" + *it + "\"\r\n");
        }
    }
}

/*
 * Register key command
 * 
 * 注册key的过程。该接口是一个内部命令，不对外使用
 * The process of registering a key. This API is Internal commands，not available to the public
 */
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

/*
 * Deregister key command
 * 
 * 注销key的过程。该接口是一个内部命令，不对外使用
 * The process of deregistering a key. This API is Internal commands，not available to the public
 */
void DeregisterKeyCmd::Execute(const std::vector<std::string>& cmd, std::string& result)
{
    if(!IsExist(cmd[1]))
    {
        result.assign("[ERROR]: key is not Exist\r\n");
    }
    else
    {
        keys_->erase(cmd[1]);
        result.assign("(integer) 1\r\n");
    }
}