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
    TString(std::shared_ptr<RBTree> rbtree) : rbtree_(rbtree) { }
    ~TString() = default;

protected:
    std::shared_ptr<RBTree> rbtree_;
};

/* ----------------------SET command------------------------- */
class SetStringCmd : public TString
{
public:
    SetStringCmd(std::shared_ptr<RBTree> rbtree) : TString(rbtree) { }
    ~SetStringCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/* ----------------------GET command------------------------- */
class GetStringCmd : public TString
{
public:
    GetStringCmd(std::shared_ptr<RBTree> rbtree) : TString(rbtree) { }
    ~GetStringCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/* ----------------------DELSTRING command------------------------- */
class DelStringCmd : public TString
{
public:
    DelStringCmd(std::shared_ptr<RBTree> rbtree) : TString(rbtree) { }
    ~DelStringCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

#endif