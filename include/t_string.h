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
    TString(std::shared_ptr<RBTree> rbtree);
    ~TString();

protected:
    std::shared_ptr<RBTree> rbtree_;
};

/* ----------------------SET command------------------------- */
class SetStringCmd : public TString
{
public:
    SetStringCmd(std::shared_ptr<RBTree> rbtree);
    ~SetStringCmd() = default;

    std::string Execute(const std::vector<std::string>& cmd);
};

/* ----------------------GET command------------------------- */
class GetStringCmd : public TString
{
public:
    GetStringCmd(std::shared_ptr<RBTree> rbtree);
    ~GetStringCmd() = default;

    std::string Execute(const std::vector<std::string>& cmd);
};

#endif