#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <random>

static const int SKIPLIST_MAXLEVEL = 32;
static const int SKIPLIST_P = 4;

typedef std::string SKIPLIST_KEY_TYPE;     // key类型
typedef std::string SKIPLIST_VALUE_TYPE;   // value类型

struct SkipListNode
{
	typedef SKIPLIST_KEY_TYPE	key_type;
	typedef SKIPLIST_KEY_TYPE	value_type;

	key_type key_;
	value_type value_;
	struct SkipLevel
	{
		SkipListNode* forward_;
	} *level_;

	SkipListNode() : level_(nullptr) { }

	SkipListNode(const key_type& key, const value_type& value, SkipListNode* const next = nullptr)
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
	typedef SkipListNode::key_type    key_type;
	typedef SkipListNode::value_type  value_type;

public:
	SkipList();
	~SkipList();
	void Insert(const key_type& key, const value_type& value);
	bool Delete(const key_type& key);
	value_type& operator[](const key_type& key);
	bool Find(const key_type& key);

protected:
	int RandomLevel();
	SkipListNode* FindNode(const key_type& key);

private:
	SkipListNode* tail_;		// 跳表尾指针
	SkipListNode* head_;		// 跳表头指针
	unsigned long long count;	// 跳表中结点个数
	int level_;					// 跳表中层数最大的节点层数
};

#endif