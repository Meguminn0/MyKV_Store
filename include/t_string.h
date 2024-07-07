/*
 * String 命令类型，值Value是字符串类型，对string类型进行操作
 * 
 * 支持的命令有：
 * 1) SET <key> <value>
 * 2) GET <key>
 *
 */

#ifndef T_STRING_H_
#define T_STRING_H_

#include <string>
#include <memory>

#include "rbtree.h"
#include "cmdstrategy.h"
#include "cmdfactory.h"

// string 命令类型初始化函数
void StringCmdInit();

class StringCmd : public CmdStrategy
{
public:
    StringCmd(const std::shared_ptr<RBTree>& rbtree) : rbtree_(rbtree) { }
    ~StringCmd() = default;

protected:
    std::shared_ptr<RBTree> rbtree_;
};

/*
 * SET <key> <value>
 * 
 * SET命令执行过程
 * SET command execution process
 */
class SetStringCmd : public StringCmd
{
public:
    SetStringCmd(const std::shared_ptr<RBTree>& rbtree) : StringCmd(rbtree) { }
    ~SetStringCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/*
 * GET <key>
 * 
 * GET命令执行过程
 * GET command execution process
 */
class GetStringCmd : public StringCmd
{
public:
    GetStringCmd(const std::shared_ptr<RBTree>& rbtree) : StringCmd(rbtree) { }
    ~GetStringCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/* ---------------------- 内部命令 (Internal commands) ------------------------- */

/*
 * DELSTRING command
 * 
 * 删除key的过程。该接口是一个内部命令，不对外使用
 * The process of delete a key. This API is Internal commands，not available to the public
 */
class DelStringCmd : public StringCmd
{
public:
    DelStringCmd(const std::shared_ptr<RBTree>& rbtree) : StringCmd(rbtree) { }
    ~DelStringCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/* ---------------------------------------------------------------------------- */

#endif