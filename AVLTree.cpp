#include "AVLTree.hpp"

AVLTree::AVLTree()
{
    root = nullptr;
}

void AVLTree::Insert(int _score, std::string _player_id)
{
    Node* node_to_insert = new Node(_score, _player_id);
    // retrace stack for updating after insertion
    std::stack<Node*> retrace;
    if(root == nullptr)
    {
        root = node_to_insert;
    }
    else
    {
        Node* current = root;
        // serch for null leaf to place new node, then rebalance using retrace stack
        while(current != nullptr)
        {
            retrace.push(current);
            if(_score < current->score)
            {
                // check for nullptr of current
                if(current->left_child == nullptr)
                {
                    current->left_child = node_to_insert;
                    retrace.push(node_to_insert);
                    break;
                }
                // move left when less than current
                current = current->left_child;
            }
            else if(_score > current->score)
            {
                // check for nullptr of current
                if(current->right_child == nullptr)
                {
                    current->right_child = node_to_insert;
                    retrace.push(node_to_insert);
                    break;
                }
                // move right when greater than current
                current = current->right_child;
            }
            // tie case (decides off of player id)
            else if(_score == current->score)
            {
                // check player ids
                // smaller player_id precedes larger ones, so if player_id larger go left
                if(std::stoi(_player_id) > std::stoi(current->player_id))
                {
                    if(current->left_child == nullptr)
                    {
                        current->left_child = node_to_insert;
                        retrace.push(node_to_insert);
                        break;
                    }
                    current = current->left_child;
                }
                // when player_id is smaller go right
                else if (std::stoi(_player_id) < std::stoi(current->player_id))
                {
                    if(current->right_child == nullptr)
                    {
                        current->right_child = node_to_insert;
                        retrace.push(node_to_insert);
                        break;  
                    }
                    current = current->right_child;
                }
            }
        }
        Update(retrace, node_to_insert);
    }
}

void AVLTree::Delete(int _score, std::string _player_id)
{
    // search tree for given player
    // saving path to retrace with update
    Node* current = root;
    Node* to_delete = nullptr;
    std::stack<Node*> retrace;
    while(current != nullptr)
    {
        retrace.push(current);
        // move left
        if(_score < current->score)
        {
            current = current->left_child;
        }
        else if(_score > current->score)
        {
            // move right
            current = current->right_child;
        }
        else if(_score == current->score)
        {
            // if scores equal ensure its the right player_id
            if(_player_id == current->player_id)
            {
                // found correct node to delete, break from loop
                to_delete = current;
                break;
            }
            else if(std::stoi(_player_id) > std::stoi(current->player_id))
            {
                // move left (higher player_id is ranked lower)
                current = current->left_child;
            }
            else if(std::stoi(_player_id) < std::stoi(current->player_id))
            {
                current = current->right_child;
            }
        }
    }
    // Pop top node of retrace, its the node to be deleted
    retrace.pop();
    // Now retrace.top() is the parent of the node to be deleted
    // Deletion cases
    // 1: No children
    // Just delete leaft node :)
    if(to_delete->left_child == nullptr && to_delete->right_child == nullptr)
    {
        // Root edge case
        if(to_delete == root)
        {
            root = nullptr;
        }
        // remove deleted node from its parent (setting it to null)
        else if(retrace.top()->left_child == to_delete)
        {
            retrace.top()->left_child = nullptr;
        }
        else
        {
            retrace.top()->right_child = nullptr;
        }
        delete to_delete;
    }

    // 2: One child
    // Save left/right child, delete node and reattach subtree
    else if(((to_delete->left_child != nullptr) && (to_delete->right_child == nullptr)) || ((to_delete->left_child == nullptr) && (to_delete->right_child != nullptr)))
    {
        // Root edge case
        if(to_delete == root)
        {
            if(to_delete->left_child != nullptr)
            {
                root = to_delete->left_child;
            }
            else
            {
                root = to_delete->right_child;
            }
        }
        // Left child case
        else if(to_delete->left_child != nullptr)
        {
            // reassign pointers
            // check if deleted node was left or right child (reassign its child to be child of its parent)
            if(retrace.top()->left_child == to_delete)
            {
                retrace.top()->left_child = to_delete->left_child;
            }
            else
            {
                retrace.top()->right_child = to_delete->left_child;
            }
        }
        // Right child case
        else
        {
            if(retrace.top()->right_child == to_delete)
            {
                retrace.top()->right_child = to_delete->right_child;
            }
            else
            {
                retrace.top()->left_child = to_delete->right_child;
            }
        }
        // Finally delete node
        delete to_delete;
    }

    // 3: Two children
    else
    {
        // First find the largest element in the left subtree
        // Add retrace to function, it will add effected nodes to stack for retrace
        Node* _largest_left = GetLargestLeft(to_delete, retrace);
        
        // Replace node to be deleted information
        to_delete->score = _largest_left->score;
        to_delete->player_id = _largest_left->player_id;

        // Now delete the largest left node (is either degree 0 or 1)
        to_delete = _largest_left;
        
        // Pop top element (its the largest left), now top is the parent of largeset left
        retrace.pop();
        // // Case 1: 0 children
        if(to_delete->left_child == nullptr && to_delete->right_child == nullptr)
        {
            if(retrace.top()->left_child == to_delete)
            {
                retrace.top()->left_child = nullptr;
            }
            else
            {
                retrace.top()->right_child = nullptr;
            }
            delete to_delete;
        }
        // Case 2: 1 child
        else
        {
            // Left child case
            if(to_delete->left_child != nullptr)
            {
                // reassign pointers
                // check if deleted node was left or right child (reassign its child to be child of its parent)
                if(retrace.top()->left_child == to_delete)
                {
                    retrace.top()->left_child = to_delete->left_child;
                }
                else
                {
                    retrace.top()->right_child = to_delete->left_child;
                }
            }
            // Right child case
            else
            {
                if(retrace.top()->right_child == to_delete)
                {
                    retrace.top()->right_child = to_delete->right_child;
                }
                else
                {
                    retrace.top()->left_child = to_delete->right_child;
                }
            }
            // Finally delete node
            delete to_delete;
        }
    }
    UpdateDelete(retrace);
}

Node* AVLTree::GetLargestLeft(Node* _node, std::stack<Node*> &_stack)
{
    // Move left first
    // Update stack to contain these nodes as well, will be used for retracing
    _stack.push(_node);
    Node* current = _node->left_child;
    while(current != nullptr)
    {
        _stack.push(current);
        current = current->right_child;
    }

    return _stack.top();    
}


void AVLTree::Update(std::stack<Node*> &_stack, Node* _node)
{
    Node* prev = nullptr;
    Node* current = nullptr;
    Node* res = nullptr;
    bool rotated = false;
    while(!_stack.empty())
    {
        // update current
        current = _stack.top();
        // check for previous rotation
        if(rotated)
        {
            // make current nodes left/right child the recently rotated node
            if(current->left_child == prev)
            {
                current->left_child = res;
            }
            else
            {
                current->right_child = res;
            }
            rotated = false;
            res = nullptr;
        }
        if(prev != nullptr)
        {
            // update height 1 + (max height of left and right child) 
            current->height = 1 + std::max(CheckHeight(current->left_child), CheckHeight(current->right_child));
            // check for rotation cases
            // 1: LL rotation 
            if(CalculateBalance(current) > 1 && _node == current->left_child->left_child)
            {
                res = LeftLeft(current);
                // set rotated flag
                rotated = true;
            }
            // 2: RR rotation
            else if(CalculateBalance(current) < -1 && _node == current->right_child->right_child)
            {
                res = RightRight(current);
                // set rotated flag
                rotated = true;
            }

            // 3: LR rotation
            else if(CalculateBalance(current) > 1 && _node == current->left_child->right_child)
            {
                res = LeftRight(current, prev);
                rotated = true;
            }

            // 4: RL rotation
            else if(CalculateBalance(current) < -1 && _node == current->right_child->left_child)
            {
                res = RightLeft(current, prev);
                rotated = true;
            }
            else
            {
                // update size (roations take care of updating effected node sizes)
                _stack.top()->size += 1;
            }
        }
        prev = _stack.top();
        _stack.pop();
    }  
}

void AVLTree::UpdateDelete(std::stack<Node*> &_stack)
{
    Node* current = nullptr;
    Node* res = nullptr;
    Node* prev = nullptr;
    bool rotated = false;
    while(!_stack.empty())
    {
        // Update current
        current = _stack.top();
        if(rotated)
        {
            // make current nodes left/right child the recently rotated node
            if(current->left_child == prev)
            {
                current->left_child = res;
            }
            else
            {
                current->right_child = res;
            }
            rotated = false;
            res = nullptr;    
        }
        // Update height of current
        current->height = 1 + std::max(CheckHeight(current->left_child), CheckHeight(current->right_child));

        // Check for rotation cases
        if(CalculateBalance(current) > 1 && current->left_child->left_child != nullptr)
        {
            res = LeftLeft(current);
            // set rotated flag
            rotated = true;
        }
        // 2: RR rotation
        else if(CalculateBalance(current) < -1 && current->right_child->right_child != nullptr)
        {
            res = RightRight(current);
            // set rotated flag
            rotated = true;
        }

        // 3: LR rotation
        else if(CalculateBalance(current) > 1 && current->left_child->right_child != nullptr)
        {
            res = LeftRight(current, current->left_child);
            rotated = true;
        }

        // 4: RL rotation
        else if(CalculateBalance(current) < -1 && current->right_child->left_child != nullptr)
        {
            res = RightLeft(current, current->right_child);
            rotated = true;
        }
        else
        {
            // Update size (all effected nodes lose one size, since one node has been deleted)
            _stack.top()->size -= 1;
        }
        prev = _stack.top();
        _stack.pop();
    }
}

Node* AVLTree::LeftLeft(Node* _node)
{
    // nodes needed for pointer reassignment
    Node* _left = _node->left_child;
    Node* _right_child = _left->right_child;

    // assign the node as the right child its the left child
    _left->right_child = _node;
    // assign the left childs right subtree as the left subtree of the node
    _node->left_child = _right_child;

    // update heights of effected nodes
    if(_right_child != nullptr)
    {
        _right_child->height = 1 + std::max(CheckHeight(_right_child->left_child), CheckHeight(_node->right_child));
    }
    _node->height = 1 + std::max(CheckHeight(_node->left_child), CheckHeight(_node->right_child));
    _left->height = 1 + std::max(CheckHeight(_left->left_child), CheckHeight(_left->right_child));

    // update sizes of effected nodes
    // right child of left node does not change size (its the same subtree that is just moved)
    _node->size = 1 + CheckSize(_node->left_child) + CheckSize(_node->right_child);
    _left->size = 1 + CheckSize(_left->left_child) + CheckSize(_left->right_child);

    // edge case for root (point root at newly rotated node)
    if(_node == root)
    {
        root = _left; 
    }

    // return left child to be made child of next node in stack
    return _left;
}

Node* AVLTree::RightRight(Node* _node)
{
    // nodes for pointer reassignment
    Node* _right = _node->right_child;
    Node* _left_child = _right->left_child;

    // assign the node to the left child of its right child
    _right->left_child = _node;
    // assign the right childs left subtree to the right child of the node
    _node->right_child = _left_child;

    // update heights of effected nodes
    if(_left_child != nullptr)
    {
        _left_child->height = 1 + std::max(CheckHeight(_left_child->left_child), CheckHeight(_left_child->right_child));
    }
    _node->height = 1 + std::max(CheckHeight(_node->left_child), CheckHeight(_node->right_child));
    _right->height = 1 + std::max(CheckHeight(_right->left_child), CheckHeight(_right->right_child));

    // update sizes of effected nodes
    _node->size = 1 + CheckSize(_node->left_child) + CheckSize(_node->right_child);
    _right->size = 1 + CheckSize(_right->left_child) + CheckSize(_right->right_child);

    // edge case for root
    if(_node == root)
    {
        root = _right;
    }

    return _right;
}

Node* AVLTree::LeftRight(Node* _node, Node* _prev)
{
    // LeftRight = RightRight + LeftLeft
    Node* res = RightRight(_prev);
    // set result from rotation as left/right child of _node
    if(_node->left_child == _prev)
    {
        _node->left_child = res;
    }
    else
    {
        _node->right_child = res;
    }
    return LeftLeft(_node);
}

Node* AVLTree::RightLeft(Node* _node, Node* _prev)
{
    // RightLeft = LeftLeft + RightRight
    Node* res = LeftLeft(_prev);
    // set result from rotation as left/right child of _node
    if(_node->left_child == _prev)
    {
        _node->left_child = res;
    }
    else
    {
        _node->right_child = res;
    }
    return RightRight(_node);
}

// Return balance based on height of given node
int AVLTree::CalculateBalance(Node* _node)
{
    if(_node == nullptr)
    {
        return 0;
    }
    else
    {
        return (CheckHeight(_node->left_child) - CheckHeight(_node->right_child));
    }
}


// Checks height of node, ensures no access to nullptr
int AVLTree::CheckHeight(Node* _node)
{
    if(_node == nullptr)
    {
        return 0;
    }
    else
    {
        return _node->height;
    }
}

// Checks size of node, ensures no access to nullptr
int AVLTree::CheckSize(Node* _node)
{
    if(_node == nullptr)
    {
        return 0;
    }
    else
    {
        return _node->size;
    }
}

Node* AVLTree::SearchByKey(int _score, std::string _player_id)
{
    Node* current = root;
    while(current != nullptr)
    {
        if(_score < current->score)
        {
            current = current->left_child;
        }
        else if(_score > current->score)
        {
            current = current->right_child;
        }
        else if(_score == current->score)
        {
            // Ensure player IDs match
            if(_player_id == current->player_id)
            {
                return current;
            }
            else if(std::stoi(_player_id) > std::stoi(current->player_id))
            {
                current = current->left_child;
            }
            else
            {
                current = current->right_child;
            }
        }
    }
    return current;
}

int AVLTree::Rank(int _score, std::string _player_id)
{
    Node* current = root;
    // Start with rank of root
    int rank = 0;
    while(current != nullptr)
    {
        if(_score < current->score)
        {
            // When moving left (increase by current size minus left subtree)
            rank = rank + (CheckSize(current) - CheckSize(current->left_child));
            current = current->left_child;
        }
        else if(_score > current->score)
        {
            // When moving right (no change to rank)
            current = current->right_child;
        }
        else if(_score == current->score)
        {
            // Ensure player IDs match
            if(_player_id == current->player_id)
            {
                rank = rank +  (CheckSize(current) - CheckSize(current->left_child));
                return rank;
            }
            else if(std::stoi(_player_id) > std::stoi(current->player_id))
            {
                rank = rank + (CheckSize(current) - CheckSize(current->left_child));
                current = current->left_child;
            }
            else
            {
                current = current->right_child;
            }
        }
    }
    return rank;
}

Node* AVLTree::Select(int _rank)
{
    Node* current = root;
    int rank = 0;
    while(current != nullptr)
    {
        if(rank + (CheckSize(current) - CheckSize(current->left_child)) < _rank)
        {
            rank = rank + (CheckSize(current) - CheckSize(current->left_child));
            current = current->left_child;
        }
        else if(rank + (CheckSize(current) - CheckSize(current->left_child)) > _rank)
        {
            current = current->right_child;
        }
        else if(rank + (CheckSize(current) - CheckSize(current->left_child)) == _rank)
        {
            return current;
        }
    }
    return current;
}

int AVLTree::GetSize()
{
    if(root == nullptr)
    {
        return 0;
    }
    return root->size;
}

Node* AVLTree::GetRoot()
{
    return root;
}

void AVLTree::DeleteTree(Node* _node)
{
    // Deletion of tree via post order traversal
    // Base case
    if(_node == nullptr)
    {
        return;
    }
    // Delete left child
    DeleteTree(_node->left_child);
    // Delete right child
    DeleteTree(_node->right_child);
    delete _node;
}

// Destructor (deletes whole tree)
AVLTree::~AVLTree()
{
    DeleteTree(root);
}