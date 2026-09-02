#include "AVLTree.hpp"
#include <iostream>
#include <unordered_map>

class Leaderboard
{
private:
    AVLTree tree;
    std::unordered_map<std::string, int> hash_table;
public:
    Leaderboard();
    std::string InsertPlayer(int _score, std::string _player_id);
    std::string RemovePlayer(std::string _player_id);
    std::string AddScore(int _added_score, std::string _player_id);
    std::string GetRank(std::string _player_id);
    std::string GetPlayerWithRank(int _rank);
    std::string PrintTopK(int _k);
};
