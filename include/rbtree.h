#ifndef RBTREE_H_
#define RBTREE_H_
#include <cassert>
#include <cstring>
// #define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

typedef bool rbtree_node_color_type;        // 节点颜色
static constexpr rbtree_node_color_type rbtree_node_red = true;      // 红色节点 
static constexpr rbtree_node_color_type rbtree_node_black = false;   // 黑色节点

typedef char* rbTree_node_key_type;     // key类型
typedef char* rbTree_node_value_type;   // value类型

struct RBTreeNode
{
    typedef rbtree_node_color_type  color_type;
    typedef rbTree_node_key_type    key_type;
    typedef rbTree_node_value_type  value_type;

    RBTreeNode* parent;     // 父节点
    RBTreeNode* left;       // 左节点
    RBTreeNode* right;      // 右结点
    color_type color;       // 节点颜色

    key_type    key;    // 节点key值
    value_type  value;  // 节点value值
};

// tree algorithm
RBTreeNode* CreateNode(RBTreeNode::key_type key, RBTreeNode::value_type value);
void InvertColor(RBTreeNode* node);

#ifdef DEBUG
class RBTree;
void show(RBTree* tree);
#endif

class RBTree
{
public:
    typedef RBTreeNode::key_type    key_type;
    typedef RBTreeNode::value_type  value_type;

    RBTree() : root_(nullptr), count_(0) { }
    ~RBTree();
    
#ifdef DEBUG
	void intrav(std::vector<char*>& v);
#endif
    int Insert(key_type key, value_type value);
    int Remove(key_type key);
    RBTreeNode* Search(key_type key);

protected:
    RBTreeNode* GetBrother(RBTreeNode* node);
    RBTreeNode* GetLeftMostNode(RBTreeNode* node);
    RBTreeNode* GetRightMostNode(RBTreeNode* node);

    void RightRotate(RBTreeNode* node);
    void LeftRotate(RBTreeNode* node);

    void LL_InsertRotate(RBTreeNode* node);
    void RR_InsertRotate(RBTreeNode* node);
    void LR_InsertRotate(RBTreeNode* node);
    void RL_InsertRotate(RBTreeNode* node);
    
    void LL_RemoveRotate(RBTreeNode* node);
    void RR_RemoveRotate(RBTreeNode* node);
    void LR_RemoveRotate(RBTreeNode* node);
    void RL_RemoveRotate(RBTreeNode* node);

    void MaintainAfterInsert(RBTreeNode* node);
    void MaintainAfterRemove(RBTreeNode* node);

private:
    RBTreeNode* root_;  // 根节点
    size_t count_;      // 所有节点个数
};

#endif