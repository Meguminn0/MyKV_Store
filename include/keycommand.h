/*
 * Key 命令类型，对所有key值进行操作
 * 
 * 支持的命令有：
 * 1) DEL <key>
 * 2) KEYS <pattern>
 * 3）EXISTS <key>
 *
 */

#ifndef KEY_COMMAND_H_
#define KEY_COMMAND_H_

#include <set>
#include <string>
#include <memory>

#include "cmdstrategy.h"
#include "cmdfactory.h"

// Key 命令类型初始化函数
void KeyCmdInit();

class KeyCmd : public CmdStrategy
{
public:
    KeyCmd(std::shared_ptr<std::set<std::string>>& shared_ptr) : keys_(shared_ptr) {}
    ~KeyCmd() = default;

    bool IsExist(const std::string& key);

protected:
    std::shared_ptr<std::set<std::string>> keys_;
};

/*
 * DEL <key>
 * 
 * DEL命令执行过程
 * DEL command execution process
 */
class DelKeyCmd : public KeyCmd
{
public:
    DelKeyCmd(std::shared_ptr<std::set<std::string>>& shared_ptr) : KeyCmd(shared_ptr) {}
    ~DelKeyCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/*
 * KEYS <pattern>
 * 
 * KEYS命令执行过程
 * KEYS command execution process
 */
class KeysKeyCmd : public KeyCmd
{
public:
    KeysKeyCmd(std::shared_ptr<std::set<std::string>>& shared_ptr) : KeyCmd(shared_ptr) {}
    ~KeysKeyCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/*
 * EXISTS <key>
 * 
 * EXISTS命令执行过程
 * EXISTS command execution process
 */
class ExistsKeyCmd : public KeyCmd
{
public:
    ExistsKeyCmd(std::shared_ptr<std::set<std::string>>& shared_ptr) : KeyCmd(shared_ptr) {}
    ~ExistsKeyCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/*------------------- 内部命令 (Internal commands) -----------------------*/

/*
 * Register key command
 * 
 * 注册key的过程。该接口是一个内部命令，不对外使用
 * The process of registering a key. This API is Internal commands，not available to the public
 */
class RegisterKeyCmd : public KeyCmd
{
public:
    RegisterKeyCmd(std::shared_ptr<std::set<std::string>>& shared_ptr) : KeyCmd(shared_ptr) {}
    ~RegisterKeyCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/*
 * Deregister key command
 * 
 * 注销key的过程。该接口是一个内部命令，不对外使用
 * The process of deregistering a key. This API is Internal commands，not available to the public
 */
class DeregisterKeyCmd : public KeyCmd
{
public:
    DeregisterKeyCmd(std::shared_ptr<std::set<std::string>>& shared_ptr) : KeyCmd(shared_ptr) {}
    ~DeregisterKeyCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};

/* ---------------------------------------------------------------------------- */

#endif