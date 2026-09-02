#include "Leaderboard.hpp"

Leaderboard::Leaderboard()
{
    // Initialize both empty data structures for Leaderboard
    // If tree size is not 0, delete old tree first
    if(tree.GetSize() != 0)
    {
        tree.DeleteTree(tree.GetRoot());
    }
    tree = AVLTree();
    hash_table = std::unordered_map<std::string, int>();
}

std::string Leaderboard::InsertPlayer(int _score, std::string _player_id)
{
    // Check for valid player id
    if(_player_id.size() != 6)
    {
        return "Player ID Must be 6 digits"; 
    }
    else
    {
        // Attempt intsertion into hash table
        auto result = hash_table.emplace(_player_id, _score);
        // Result stores pair, second element is true if success/ false if failed
        if(result.second)
        {
            // Insert into AVL tree
            tree.Insert(_score, _player_id);
        }
        else
        {
            return "Duplicate player ID";
        }
    }
    return "";
}

std::string Leaderboard::RemovePlayer(std::string _player_id)
{
    // Find matching player and score from hash table
    auto result = hash_table.find(_player_id);

    // Result is an iterator either to the end of hash table/ or the element
    // When pointing to end element is not found
    if(result == hash_table.end())
    {
        return "Player " + _player_id + " not found.";
    }
    // Found matching player
    else
    {
        // Delete from hash table and AVL tree
        tree.Delete(result->second, result->first);
        hash_table.erase(result); 
    }
    return "";
}

std::string Leaderboard::AddScore(int _added_score, std::string _player_id)
{
    // Find matching player and score from hash table
    auto result = hash_table.find(_player_id);

    if(result == hash_table.end())
    {
        return "Player " + _player_id + " not found.";
    }
    else
    {
        // Delete player from tree
        tree.Delete(result->second, result->first);
        // Re-insert element with same id and added score
        tree.Insert((result->second + _added_score), result->first);
        // Change value at player id in hash tabel
        hash_table[_player_id] += _added_score;
    }
    return "";
}

std::string Leaderboard::GetRank(std::string _player_id)
{
    // Find matching player from hash table
    auto result = hash_table.find(_player_id);

    if(result == hash_table.end())
    {
        return "Player " + _player_id + " doesn't exist";
    }
    else
    {
        int rank = tree.Rank(result->second, result->first);
        return "get_rank() of player " + _player_id + " is " + std::to_string(rank);
    }

    return "";
}

std::string Leaderboard::GetPlayerWithRank(int _rank)
{
    // Ensure that the rank is valid
    if(_rank <= 0)
    {
        return "No player with this rank";
    }
    else if(_rank > tree.GetSize())
    {
        return  "No player with this rank";
    }
    else
    {
        Node* result = tree.Select(_rank);
        return "get_player_with_rank (" + std::to_string(_rank) + ") = (" + std::to_string(result->score) + ", " + result->player_id + ")"; 
    }
}

std::string Leaderboard::PrintTopK(int _k)
{
    // Ensure valid k
    if(_k <= 0)
    {
        return "Invalid k: k must be greater than 0 \n";
    }
    else
    {
        // Make return value string
        // Starting with original value of k
        std::string ret = "print_top_k(" + std::to_string(_k) + "):" + "\n";
        // Find min bewteen k and n
        int k = std::min(_k, tree.GetSize());
        for(int i = 1; i <= k; i++)
        {
            // Get the player with that rank
            Node* result = tree.Select(i);
            // Add to return string
            std::string res = "-> (" + result->player_id + ", " + std::to_string(result->score) + "), " + std::to_string(i) + "\n";
            ret += res;
        }
        return ret;
    }
}