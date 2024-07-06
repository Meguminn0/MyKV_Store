#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <random>

static const int SKIPLIST_MAXLEVEL = 32;
static const int SKIPLIST_P = 4;

struct SkipListNode
{
	int key_;
	int value_;
	struct SkipLevel
	{
		SkipListNode* forward_;
	} *level_;

	SkipListNode() : key_(0), value_(0), level_(nullptr) { }

	SkipListNode(const int& key, const int& value, SkipListNode* const next = nullptr)
		: key_(key), value_(value)
	{
		level_ = new SkipLevel[SKIPLIST_MAXLEVEL + 1];
		for (int i = 0; i < SKIPLIST_MAXLEVEL + 1; ++i)
		{
			level_[i].forward_ = next;
		}
	}

	~SkipListNode()
	{
		if (level_ != nullptr)
		{
			delete[] level_;
		}
	}
};

class SkipList
{
public:
	SkipList();
	~SkipList();
	void Insert(const int& key, const int& value);
	bool Delete(const int& key);
	bool Find(const int& key);

protected:
	int RandomLevel();
	SkipListNode* FindNode(const int& key);

private:
	SkipListNode* tail_;		// 跳表尾指针
	SkipListNode* head_;		// 跳表头指针
	unsigned long long count;	// 跳表中结点个数
	int level_;					// 跳表中层数最大的节点层数
};


#endif