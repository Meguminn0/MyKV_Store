#ifndef RBTREE_H_
#define RBTREE_H_

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
RBTreeNode* CreateNode(RBTreeNode::key_type key, RBTreeNode::value_type value)
{
    std::allocator<RBTreeNode> alltor;
    RBTreeNode* node = new RBTreeNode();
    node->key = key
    node->value = value;
    node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->color = rbtree_node_red:
    
    return node;
}

void InvertColor(RBTreeNode* node)
{
    node->color = !node->color;
}

RBTreeNode* GetBrother(RBTreeNode* node)
{
    bool is_left_node = ((node == node->parent->left) ? true : false);
    if(is_left_node)
    {
        return node->parent->right;
    }
    else
    {
        return node->parent->left;
    }
}

RBTreeNode* GetLeftMostNode(RBTreeNode* node)
{
    while(node->left != nullptr)
    {
        node = node->left;
    }

    return node;
}

RBTreeNode* GetRightMostNode(RBTreeNode* node)
{
    while(node->right != nullptr)
    {
        node = node->right;
    }

    return node;
}

LL_InsertRotate(RBTreeNode* node)
{

}

RR_InsertRotate(RBTreeNode* node)
{
    
}

LR_InsertRotate(RBTreeNode* node)
{
    
}

RL_InsertRotate(RBTreeNode* node)
{
    
}

void MaintainAfterInsert(RBTreeNode* node)
{
    if(node->parent == nullptr && node->color == rbtree_node_red)
    {
        InvertColor(node);
        return;
    }

    RBTreeNode* parent = node->parent;
    assert(parent != nullptr);

    if(parent->color == rbtree_node_red)
    {
        RBTreeNode* grandparent = parent->parent;
        assert(grandparent != nullptr);
        RBTreeNode* uncle = GetBrother(parent);

        if(uncle != nullptr && uncle->color == rbtree_node_red)
        {
            InvertColor(uncle);
            InvertColor(parent);
            InvertColor(grandparent);

            MaintainAfterInsert(grandparent);
        }
        else
        {
            bool parent_is_left = ((parent == grandparent->left) ? true : false);
            bool node_is_left = ((node == parent->left) ? true : false);
            if(parent_is_left)
            {
                if(node_is_left)
                {
                    LL_InsertRotate(node);
                }
                else
                {
                    LR_InsertRotate(node);
                }
            }
            else
            {
                if(node_is_left)
                {
                    RL_InsertRotate(node);
                }
                else
                {
                    RR_InsertRotate(node);
                }
            }
        }
    }
}

void MaintainAfterRemove(RBTreeNode* node)
{

}

class RBTree
{
public:
    typedef RBTreeNode::key_type    key_type;
    typedef RBTreeNode::value_type  value_type;

    RBTree() : root_(nullptr), count_(0) { }
    ~RBTree();

    int Insert(key_type key, value_type value);
    int remove(key_type key);
    RBTreeNode* search(key_type key);

private:
    RBTreeNode* root_;  // 根节点
    size_t count_;      // 所有节点个数
};

#endif