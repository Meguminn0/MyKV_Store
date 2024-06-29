#include <string>
#include <memory>

#include "rbtree.h"
#include "t_string.h"
#include "cmdfactory.h"

#include <iostream>

void StringCmdInit(RBTree* rbtree)
{
    std::shared_ptr<RBTree> shared_rbtree(rbtree);
    CmdFactory& cmd_factory = CmdFactory::GetInstance();
    
    cmd_factory.RegisterCmdStrategy("GET", new GetStringCmd(shared_rbtree));
    cmd_factory.RegisterCmdStrategy("SET", new SetStringCmd(shared_rbtree));
}

TString::TString(std::shared_ptr<RBTree> rbtree) : rbtree_(rbtree)
{

}

TString::~TString()
{

}

/* ----------------------SET command------------------------- */
SetStringCmd::SetStringCmd(std::shared_ptr<RBTree> rbtree) : TString(rbtree)
{

}

std::string SetStringCmd::Execute(const std::vector<std::string>& cmd)
{
    int idx = RES_ERROR;
    if(cmd.size() == 3)
    {
        int flag = rbtree_->Insert(const_cast<char*>(cmd[1].c_str()), const_cast<char*>(cmd[2].c_str()));

        if(flag == 1)
        {
            idx = RES_OK;
        }
        else if(flag == 0)
        {
            idx = RES_FAIL;
        }
        else
        {
            idx = RES_AL_HAVE;
        }
    }

    return RES_MSG[idx];
}

/* ----------------------GET command------------------------- */
GetStringCmd::GetStringCmd(std::shared_ptr<RBTree> rbtree) : TString(rbtree)
{

}

std::string GetStringCmd::Execute(const std::vector<std::string>& cmd)
{
    if(cmd.size() == 2)
    {
        std::string str;
        str = rbtree_->Search(cmd[1]);

        if(str.empty())
        {
            str = "nil";
            return "(" + str + ")\r\n";
        }
        else
        {
            return "\"" + str + "\"\r\n";
        }
    }

    return RES_MSG[RES_ERROR];
}
