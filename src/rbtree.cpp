#include <stack>
#include "rbtree.h"

// 析构函数
RBTree::~RBTree()
{
    if(root_ != nullptr)
    {
        // 通过后序遍历delete所有节点
        std::stack<RBTreeNode*> stk1;
        std::stack<RBTreeNode*> stk2;
        stk1.push(root_);
        while(!stk1.empty())
        {
            RBTreeNode* node = stk1.top();
            stk1.pop();
            stk2.push(node);
            if(node->left != nullptr)
            {
                stk1.push(node->left);
            }
            if(node->right != nullptr)
            {
                stk1.push(node->right);
            }
        }

        while(!stk2.empty())
        {
            RBTreeNode* node = stk2.top();
            stk2.pop();
            delete node;
        }
    }
}

// 成功返回1，失败返回0, 已存在返回-2
int RBTree::Insert(key_type key, value_type value)
{
    if(root_ == nullptr)
    {
        root_ = CreateNode(key, value);
        InvertColor(root_);
        ++count_;
        return 1;
    }
    
    RBTreeNode* insert_node = root_;
    while(insert_node != nullptr)
    {
        if(strcmp(insert_node->key, key) > 0)
        {
            if(insert_node->right == nullptr)
            {
                insert_node->right = CreateNode(key, value);
                insert_node->right->parent = insert_node;
                MaintainAfterInsert(insert_node->right);
                ++count_;
                return 1;
            }
            insert_node = insert_node->right;
        }
        else if(strcmp(insert_node.key, key) < 0)
        {
            if(insert_node->left == nullptr)
            {
                insert_node->left = CreateNode(key, value);
                insert_node->left->parent = insert_node;
                MaintainAfterInsert(insert_node->left);
                ++count_;
                return 1;
            }
            insert_node = insert_node->left;
        }
        else
        {
            return -2;
        }
    }

    return 0;
}

// 返回1成功，返回0失败
int RBTree::remove(key_type key)
{
    RBTreeNode* remove_node = search(key);
    if(remove_node != nullptr)
    {
        if(remove_node->left != nullptr && remove_node->right != nullptr)
        {
            RBTreeNode* replace_node = GetLeftMostNode(remove_node->left);
            remove_node->key = replace_node->key;
            remove_node->value = replace_node->value;
            remove_node = replace_node;
        }
        
        if(remove_node->left != nullptr && remove_node->right == nullptr ||
                remove_node->left == nullptr && remove_node->right != nullptr)
        {
            RBTreeNode* parent = remove_node->parent;
            RBTreeNode* child_node = nullptr;
            bool is_left_node = ((remove_node == parent->left) ? true : false);
            if(remove_node->left != nullptr)
            {
                child_node = remove_node->left;
            }
            else
            {
                child_node = remove_node->right;
            }

            child_node->parent = parent;
            if(is_left_node)
            {
                parent->left = child_node;
            }
            else
            {
                parent->right = child_node;
            }
            InvertColor(rchild_node);
        }
        else
        {
            RBTreeNode* parent = remove_node->parent;
            bool is_left_node = ((remove_node == parent->left) ? true : false);
            if(remove_node == root_)
            {
                root_ = nullptr;
            }
            else
            {
                if(remove_node->color == rbtree_node_black)
                {
                    MaintainAfterRemove(remove_node);
                }

                if(is_left_node)
                {
                    parent->left = nullptr;
                }
                else
                {
                    parent->right = nullptr;
                }
            }
        }
        delete remove_node;
        --count_;
        return 1;
    }

    return 0;
}

// 成功返回指针，失败返回nullptr
RBTreeNode* RBTree::Search(key_type key)
{
    RBTreeNode* search_node = root_;
    while(search_node != nullptr)
    {
        if(strcmp(search_node->key, key) < 0)
        {
            search_node = search_node->left;
        }
        else if(strcmp(search_node->key, key) > 0)
        {
            search_node = search_node->right;
        }
        else
        {
            break;
        }
    }

    return search_node;
}
