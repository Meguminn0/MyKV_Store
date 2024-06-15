#ifndef RBTREE_H_
#define RBTREE_H_

typedef bool rbtree_node_color_type;        // 节点颜色
typedef rbtree_node_color_type rbtree_node_red = true;      // 红色节点 
typedef rbtree_node_color_type rbtree_node_black = false;   // 黑色节点

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

class RBTree
{
public:
    typedef RBTreeNode::key_type    key_type;
    typedef RBTreeNode::value_type  value_type;

    // 构造函数
    RBTree() : root_(nullptr), count_(0) { }
    // 析构函数
    ~RBTree();

    int Insert(key_type key, value_type value);
    int remove(key_type key);
    RBTreeNode* search(key_type key);

private:
    RBTreeNode* root_;
    size_t count_;
};

#endif