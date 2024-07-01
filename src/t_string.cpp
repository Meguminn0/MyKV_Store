#include <string>
#include <memory>

#include "rbtree.h"
#include "t_string.h"
#include "cmdfactory.h"
#include "cmdstrategy.h"

/*
 * string 命令初始化函数
 * string command init function
 */
void StringCmdInit(RBTree* rbtree)
{
    std::shared_ptr<RBTree> shared_rbtree(rbtree);
    CmdFactory& cmd_factory = CmdFactory::GetInstance();
    
    cmd_factory.RegisterCmdStrategy("GET", new GetStringCmd(shared_rbtree));
    cmd_factory.RegisterCmdStrategy("SET", new SetStringCmd(shared_rbtree));
    cmd_factory.RegisterCmdStrategy("DEL_STRING", new DelStringCmd(shared_rbtree));
}

/*
 * SET <key> <value>
 * 
 * SET命令执行过程
 * SET command execution process
 */
void SetStringCmd::Execute(const std::vector<std::string>& cmd, std::string& result)
{
    if(cmd.size() < 3)
    {
        result.assign(RES_MSG[RES_ERROR]);
        return;
    }

    int flag = rbtree_->Insert(const_cast<char*>(cmd[1].c_str()), const_cast<char*>(cmd[2].c_str()));
    if(flag == 1)
    {
        result.assign(RES_MSG[RES_OK]);
        CmdStrategy* strategy = CmdFactory::GetInstance().GetCmdStrategy("REGISTER_KEY");
        std::string res;
        strategy->Execute(cmd, res);
    }
    else if(flag == 0)
    {
        result.assign(RES_MSG[RES_FAIL]);
    }
    else
    {
        result.assign(RES_MSG[RES_AL_HAVE]);
    }
    return;
}

/*
 * GET <key>
 * 
 * GET命令执行过程
 * GET command execution process
 */
void GetStringCmd::Execute(const std::vector<std::string>& cmd, std::string& result)
{
    if(cmd.size()  < 2)
    {
        result.assign(RES_MSG[RES_ERROR]);
        return;
    }

    std::string str;
    str = rbtree_->Search(cmd[1]);

    if(str.empty())
    {
        str = "nil";
        result.assign("(" + str + ")\r\n");
    }
    else
    {
        result.assign("\"" + str + "\"\r\n");
    }
}

/*
 * DELSTRING command
 * 
 * 删除key的过程。该接口是一个内部命令，不对外使用
 * The process of delete a key. This API is Internal commands，not available to the public
 */
void DelStringCmd::Execute(const std::vector<std::string>& cmd, std::string& result)
{
    if(cmd.size() == 2)
    {
        if(rbtree_->Remove(cmd[1]) == 1)
        {
            result.assign("(integer) 1\r\n");
        }
        else
        {
            result.assign("(integer) 0\r\n");
        }
        
    }
    else
    {
        result.assign(RES_MSG[RES_ERROR]);
    }
}
