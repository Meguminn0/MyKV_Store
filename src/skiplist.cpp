#include "skiplist.h"
#include <random>

SkipList::SkipList()
{
    srand(time(nullptr));
    tail_ = nullptr;
    head_ = new SkipListNode(0, 0, tail_);
    count = 0;
    level_ = 0;
}

SkipList::~SkipList()
{
    SkipListNode* p = head_;
    while (head_ != nullptr)
    {
        head_ = p->level_[0].forward_;
        delete p;
        p = head_;
    }
}

void SkipList::Insert(const int& key, const int& value)
{
    SkipListNode* update[SKIPLIST_MAXLEVEL + 1], * p;
    p = head_;

    // 逐步降级查找目标节点，记录搜索路径
    for (int i = level_; i >= 0; --i)
    {
        while (p->level_[i].forward_ != tail_ && p->level_[i].forward_->key_ < key)
        {
            p = p->level_[i].forward_;
        }
        // 记录搜索路径, 降到下一级
        update[i] = p;
    }

    // 当前跳表中存在键值为key的节点，更新对应的value值。
    if (p->key_ == key)
    {
        p->value_ = value;
        return;
    }

    int level = RandomLevel();
    if (level > level_)
    {
        /* 当新增节点随机的层数大于当前表中最大的层数时，更新最大层的路径为head开始。
            *                                   \/-------------- update[level]
            *      | 3 | --------------------- |  | ---- |  | 
            * head | 2 | ---------- |  | ----- |  | ---- |  | 
            *      | 1 | -- |  | -- |  | ----- |  | ---- |  | 
            *      | 0 | -- node1 -- node2 -- newnode -- tail
            */
        ++level_;
        level = level_;
        update[level_] = head_;
    }

    SkipListNode* newNode = new SkipListNode(key, value);
    // 将新节点插入到跳表中，需要同时维护新节点和其level数组指针
    for (int i = 0; i < level; ++i)
    {
        newNode->level_[i].forward_ = update[i]->level_[i].forward_;
        update[i]->level_[i].forward_ = newNode;
    }
    ++count;
}

bool SkipList::Delete(const int& key)
{
    SkipListNode* update[SKIPLIST_MAXLEVEL + 1], * p;

    p = head_;
    for (int i = level_; i >= 0; --i)
    {
        while (p->level_[i].forward_ != nullptr && p->level_[i].forward_->key_ < key)
        {
            p = p->level_[i].forward_;
        }
        update[i] = p;
    }
    p = p->level_[0].forward_;

    if (p == nullptr || p->key_ != key)
    {
        return false;
    }

    for (int i = 0; i <= level_; ++i)
    {
        if (update[i]->level_[i].forward_ != p)
        {
            break;
        }

        update[i]->level_[i].forward_ = p->level_[i].forward_;
    }

    delete p;
    while (level_ > 1 && head_->level_[level_].forward_ == tail_)
    {
        --level_;
    }

    --count;
    return true;
}

bool SkipList::Find(const int& key)
{
    if (FindNode(key) != nullptr)
    {
        return true;
    }

    return false;
}

int SkipList::RandomLevel()
{
    int level = 1;
    while ((rand() & 0xFFFF) < 0xFFFF / SKIPLIST_P)
    {
        ++level;
    }

    return SKIPLIST_MAXLEVEL < level ? SKIPLIST_MAXLEVEL : level;
}

SkipListNode* SkipList::FindNode(const int& key)
{
    SkipListNode* p = head_;
    for (int i = level_; i >= 0; --i)
    {
        while (p->level_[i].forward_ != nullptr && p->level_[i].forward_->key_ < key)
        {
            p = p->level_[i].forward_;
        }
    }
    p = p->level_[0].forward_;

    if (p != nullptr && p->key_ == key)
    {
        return p;
    }
    return nullptr;
}
