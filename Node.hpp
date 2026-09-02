#include <string>

struct Node 
{
    int size; // size of subtree
    std::string player_id; // unique
    int score; // non-unique used for insertion
    int height; // for calculating balance
    Node* left_child;
    Node* right_child;

    // constructor
    Node(int _score, std::string _player_id);
};

