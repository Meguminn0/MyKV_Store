#include <stack>
#include <cassert>
#include <cstring>
#include <vector>
#include "rbtree.h"

RBTreeNode* CreateNode(RBTreeNode::key_type key, RBTreeNode::value_type value)
{
    RBTreeNode* node = new RBTreeNode();
    node->key = key;
    node->value = value;
    node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->color = rbtree_node_red;
    
    return node;
}

void InvertColor(RBTreeNode* node)
{
    node->color = !node->color;
}

#ifdef DEBUG
void show(RBTree* tree)
{
	std::vector<std::string> retv;
	tree->intrav(retv);
	if (retv.empty())
	{
		std::cout << "空树" << std::endl;
	}
	else
	{
		for (auto item : retv)
		{
			std::cout << item << ' ';
		}
		puts("");
	}
}
#endif

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
        if(key > insert_node->key)
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
        else if(key < insert_node->key)
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
int RBTree::Remove(key_type key)
{
    RBTreeNode* remove_node = SearchNode(key);
    if(remove_node != nullptr)
    {
        if(remove_node->left != nullptr && remove_node->right != nullptr)
        {
            RBTreeNode* replace_node = GetRightMostNode(remove_node->left);
            remove_node->key = replace_node->key;
            remove_node->value = replace_node->value;
            remove_node = replace_node;
        }
        
        if(remove_node->left != nullptr && remove_node->right == nullptr ||
                remove_node->left == nullptr && remove_node->right != nullptr)
        {
           RBTreeNode* parent = remove_node->parent;
			RBTreeNode* child_node = ((remove_node->left == nullptr) ? remove_node->right : remove_node->left);

			InvertColor(child_node);
			child_node->parent = parent;
			if (parent == nullptr)
			{
				root_ = child_node;
			}
			else if (remove_node == parent->left)
			{
				parent->left = child_node;
			}
			else
			{
				parent->right = child_node;
			}
        }
        else
        {
            RBTreeNode* parent = remove_node->parent;
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

                if(remove_node == parent->left)
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

// 成功返回字符串，失败返回空字符串
RBTree::value_type RBTree::Search(key_type key)
{
    RBTreeNode* search_node = SearchNode(key);
    if(search_node != nullptr)
    {
        return search_node->value;
    }

    return "";
}

// 成功返回指针，失败返回nullptr
RBTreeNode* RBTree::SearchNode(key_type key)
{
    RBTreeNode* search_node = root_;
    while(search_node != nullptr)
    {
        if(key < search_node->key)
        {
            search_node = search_node->left;
        }
        else if(key > search_node->key)
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

RBTreeNode* RBTree::GetBrother(RBTreeNode* node)
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

RBTreeNode* RBTree::GetLeftMostNode(RBTreeNode* node)
{
    while(node->left != nullptr)
    {
        node = node->left;
    }

    return node;
}

RBTreeNode* RBTree::GetRightMostNode(RBTreeNode* node)
{
    while(node->right != nullptr)
    {
        node = node->right;
    }

    return node;
}


void RBTree::RightRotate(RBTreeNode* node)
{
    assert(node != nullptr && node->left != nullptr);

    RBTreeNode* child = node->left;
    node->left = child->right;
    child->right = node;
    child->parent = node->parent;
    node->parent = child;

    if(node->left != nullptr)
    {
        node->left->parent = node;
    }

    if(child->parent == nullptr)
    {
        root_ = child;
    }
    else if(child->parent->left == node)
    {
        child->parent->left = child;
    }
    else
    {
        child->parent->right = child;
    }
}

#ifdef DEBUG
void RBTree::intrav(std::vector<std::string>& v)
{
	RBTreeNode* head = this->root_;
	if (head != nullptr)
	{
		std::stack<RBTreeNode*> stk;
		while (!stk.empty() || head != nullptr)
		{
			if (head != nullptr)
			{
				stk.push(head);
				head = head->left;
			}
			else
			{
				head = stk.top();
				stk.pop();
				v.push_back(head->value);
				head = head->right;
			}
		}
	}
}
#endif

void RBTree::LeftRotate(RBTreeNode* node)
{
    assert(node != nullptr && node->right != nullptr);

    RBTreeNode* child = node->right;
    node->right = child->left;
    child->left = node;
    child->parent = node->parent;
    node->parent = child;

    if(node->right != nullptr)
    {
        node->right->parent = node;
    }

    if(child->parent == nullptr)
    {
        root_ = child;
    }
    else if(child->parent->left == node)
    {
        child->parent->left = child;
    }
    else
    {
        child->parent->right = child;
    }
}


void RBTree::LL_InsertRotate(RBTreeNode* node)
{
    InvertColor(node->parent);
    InvertColor(node->parent->parent);
    RightRotate(node->parent->parent);
}

void RBTree::RR_InsertRotate(RBTreeNode* node)
{
    InvertColor(node->parent);
    InvertColor(node->parent->parent);
    LeftRotate(node->parent->parent);
}

void RBTree::LR_InsertRotate(RBTreeNode* node)
{
    LeftRotate(node->parent);
    InvertColor(node);
    InvertColor(node->parent);
    RightRotate(node->parent);
}

void RBTree::RL_InsertRotate(RBTreeNode* node)
{
    RightRotate(node->parent);
    InvertColor(node);
    InvertColor(node->parent);
    LeftRotate(node->parent);
}


void RBTree::LL_RemoveRotate(RBTreeNode* node)
{
    RBTreeNode* parent = node->parent;
    RBTreeNode* grandparent = parent->parent;
    node->color = parent->color;
    parent->color = grandparent->color;
    grandparent->color = rbtree_node_black;

    RightRotate(grandparent);
}

void RBTree::RR_RemoveRotate(RBTreeNode* node)
{
    RBTreeNode* parent = node->parent;
    RBTreeNode* grandparent = parent->parent;
    node->color = parent->color;
    parent->color = grandparent->color;
    grandparent->color = rbtree_node_black;

    LeftRotate(grandparent);
}

void RBTree::LR_RemoveRotate(RBTreeNode* node)
{
    RBTreeNode* parent = node->parent;
    RBTreeNode* grandparent = parent->parent;
    node->color = grandparent->color;
    grandparent->color = rbtree_node_black;

    LeftRotate(parent);
    RightRotate(grandparent);
}

void RBTree::RL_RemoveRotate(RBTreeNode* node)
{
    RBTreeNode* parent = node->parent;
    RBTreeNode* grandparent = parent->parent;
    node->color = grandparent->color;
    grandparent->color = rbtree_node_black;

    RightRotate(parent);
    LeftRotate(grandparent);
}


void RBTree::MaintainAfterInsert(RBTreeNode* node)
{
    while(node->parent != nullptr && node->parent->color == rbtree_node_red)
    {
        RBTreeNode* parent = node->parent;
        RBTreeNode* grandparent = parent->parent;
        assert(grandparent != nullptr);
        RBTreeNode* uncle = GetBrother(parent);

        if(uncle != nullptr && uncle->color == rbtree_node_red)
        {
            InvertColor(uncle);
            InvertColor(parent);
            InvertColor(grandparent);

            node = grandparent;
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
    
    if(node->parent == nullptr && node->color == rbtree_node_red)
    {
        InvertColor(node);
    }
}

void RBTree::MaintainAfterRemove(RBTreeNode* node)
{
    while(node != nullptr && node->color == rbtree_node_black)
    {
        RBTreeNode* parent = node->parent;
        bool node_is_left = ((node == parent->left) ? true : false);
        RBTreeNode* brother = (node_is_left ? parent->right : parent->left);

        if(brother->color == rbtree_node_red)
        {
            InvertColor(brother);
            InvertColor(parent);
            if(node_is_left)
            {
                LeftRotate(parent);
            }
            else
            {
                RightRotate(parent);
            }
        }
        else
        {
            RBTreeNode* left_node = brother->left;
            RBTreeNode* right_node = brother->right;
            bool left_node_is_black = (left_node == nullptr || left_node->color == rbtree_node_black);
            bool right_node_is_black = (right_node == nullptr || right_node->color == rbtree_node_black);

            if(left_node_is_black && right_node_is_black)
            {
                InvertColor(brother);
                if(parent->parent == nullptr)
                {
                    break;
                }
                node = parent;
            }
            else
            {
                if(node_is_left)
                {
                    if(!right_node_is_black)
                    {
                        RR_RemoveRotate(right_node);
						break;
                    }
                    else
                    {
                        RL_RemoveRotate(left_node);
						break;
                    }
                }
                else
                {
                    if(!left_node_is_black)
                    {
                        LL_RemoveRotate(left_node);
						break;
                    }
                    else
                    {
                        LR_RemoveRotate(right_node);
						break;
                    }
                }
            }
        }
    }

    if(node != nullptr && node->color == rbtree_node_red)
    {
        InvertColor(node);
    }
}

