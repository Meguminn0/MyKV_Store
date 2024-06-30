#include <string>
#include <memory>

#include "rbtree.h"
#include "t_string.h"
#include "cmdfactory.h"
#include "cmdstrategy.h"

#include <iostream>

void StringCmdInit(RBTree* rbtree)
{
    std::shared_ptr<RBTree> shared_rbtree(rbtree);
    CmdFactory& cmd_factory = CmdFactory::GetInstance();
    
    cmd_factory.RegisterCmdStrategy("GET", new GetStringCmd(shared_rbtree));
    cmd_factory.RegisterCmdStrategy("SET", new SetStringCmd(shared_rbtree));
    cmd_factory.RegisterCmdStrategy("DELSTRING", new DelStringCmd(shared_rbtree));
}

/* ----------------------SET command------------------------- */
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
        CmdStrategy* strategy = CmdFactory::GetInstance().GetCmdStrategy("REGISTERKEY");
        std::string result;
        strategy->Execute(cmd, result);
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

/* ----------------------GET command------------------------- */
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

/* ----------------------DELSTRING command------------------------- */
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
