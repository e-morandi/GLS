#include "Node.hpp"
#include <iostream>
#include <stack>
#include <algorithm>

class AVLTree
{
    private:
        // root node
        Node* root;
    public:
        AVLTree();
        ~AVLTree();
        void Insert(int _score, std::string _player_id);
        void Delete(int _score, std::string _player_id);
        Node* SearchByKey(int _score, std::string _player_id);
        int Rank(int _score, std::string _player_id);
        Node* Select(int _rank);
        int GetSize();
        void DeleteTree(Node* _node);
        Node* GetRoot();
    private:

        void Update(std::stack<Node*> &_stack, Node* _node);
        void UpdateDelete(std::stack<Node*> &_stack);
        Node* LeftLeft(Node* _node);
        Node* RightRight(Node* _node);
        Node* LeftRight(Node* _node, Node* _prev);
        Node* RightLeft(Node* _node, Node* _prev);
        int CalculateBalance(Node* _node);
        int CheckHeight(Node* _node);
        int CheckSize(Node* _node);
        Node* GetLargestLeft(Node* _node, std::stack<Node*> &_stack);
};