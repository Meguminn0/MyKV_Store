/*
 * String 命令类型，值Value是字符串类型
 */

#ifndef T_STRING_H_
#define T_STRING_H_

#include <string>
#include <memory>
#include <rbtree.h>

#include "cmdstrategy.h"
#include "cmdfactory.h"

// string 命令类型初始化函数
void StringCmdInit(RBTree* rbtree);

class TString : public CmdStrategy
{
public:
    TString(RBTree* rbtree);
    ~TString();

protected:
    std::shared_ptr<RBTree> rbtree_;
};

class SetStringCmd : public TString
{
public:
    SetStringCmd(RBTree* rbtree);
    ~SetStringCmd() = default;

    std::string Execute(const std::vector<std::string>& cmd);
};

class GetStringCmd : public TString
{
public:
    GetStringCmd(RBTree* rbtree);
    ~GetStringCmd() = default;

    std::string Execute(const std::vector<std::string>& cmd);
};

#endif