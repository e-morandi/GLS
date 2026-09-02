#include "Node.hpp"

// constructor
Node::Node(int _score, std::string _player_id)
{
    size = 1;
    score = _score;
    player_id = _player_id;
    left_child = nullptr;
    right_child = nullptr;
    height = 1;
}