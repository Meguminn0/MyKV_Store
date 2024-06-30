#ifndef KEY_COMMAND_H_
#define KEY_COMMAND_H_

#include <set>
#include <string>
#include <memory>

#include "cmdstrategy.h"
#include "cmdfactory.h"

void KeyCmdInit(std::set<std::string>* pkeys_set);

class KeyCmd : public CmdStrategy
{
public:
    KeyCmd(std::shared_ptr<std::set<std::string>>& shared_ptr) : keys_(shared_ptr) {}
    ~KeyCmd() = default;

    bool IsExist(const std::string& key);

protected:
    std::shared_ptr<std::set<std::string>> keys_;
};

/*-------------------DEL command-----------------------*/
class DelKeyCmd : public KeyCmd
{
public:
    DelKeyCmd(std::shared_ptr<std::set<std::string>>& shared_ptr) : KeyCmd(shared_ptr) {}
    ~DelKeyCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};


/*-------------------Internal commands-----------------------*/
class RegisterKeyCmd : public KeyCmd
{
public:
    RegisterKeyCmd(std::shared_ptr<std::set<std::string>>& shared_ptr) : KeyCmd(shared_ptr) {}
    ~RegisterKeyCmd() = default;

    void Execute(const std::vector<std::string>& cmd, std::string& result);
};
/*-------------------DEL command-----------------------*/

#endif