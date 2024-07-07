/*
 * Zset 命令类型，对有序集合进行操作
 * 
 * 支持的命令有：
 * 1) 
 * 2) 
 * 3）
 *
 */

#ifndef T_ZSET_H_
#define T_ZSET_H_

#include <memory>

#include "cmdstrategy.h"
#include "skiplist.h"

// Zset 命令类型初始化函数
void ZsetCmdInit();

class ZsetCmd : public CmdStrategy
{
public:
    ZsetCmd(std::shared_ptr<SkipList>& skiplist) : skiplist_(skiplist) {}
    ~ZsetCmd() = default;

protected:
    std::shared_ptr<SkipList> skiplist_;
};

#endif