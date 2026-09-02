#include <iostream>
#include <fstream>
#include "Leaderboard.hpp"
int main(int argc, char* argv[])
{
    // Ensure that suffcient agruments were passed
    if(argc != 2)
    {
        std::cout << "No input file" << std::endl;
    }
    else
    {
        // Open Input file
        std::ifstream file(argv[1]);
        
        // Create Output file name from input file name 
        std::string input_file_name = argv[1];
        int index_of_dot =  input_file_name.find_last_of(".");
        std::string output_file_name = input_file_name.substr(0, index_of_dot) + "_" + "output_file.txt";
        // Create Output file
        std::ofstream out_file(output_file_name);
        // String for reading lines of input
        std::string line = "";

        // Create empty Leaderboard
        Leaderboard leaderboard;
        // Ensure that the file exists
        if(file.is_open())
        {
            bool reading = true;
            while(reading)
            {
                // Get each line and run commands based on input
                while(getline(file, line))
                {
                    // Initilization
                    if(line == "initialize()")
                    {
                        // Run intialize function
                        leaderboard = Leaderboard();
                    }
                    // Inserting player
                    else if(line.find("insert_player") != std::string::npos)
                    {
                        // Get score and player id
                        int index_parent_first = line.find_first_of("(");
                        int index_comma = line.find(",");
                        int index_parent_last = line.find(")");
                        bool valid = true;
                        int score;
                        // Score is index of first paranthese + 1 untill comma
                        try
                        {
                            score = std::stoi(line.substr(index_parent_first + 1, (index_comma - index_parent_first + 1)));
                        }
                        catch(const std::invalid_argument& e)
                        {
                            valid = false;
                        }
                        // Player id is index comma + 2 untill last parathese
                        std::string player_id = line.substr(index_comma + 2, (index_parent_last - index_comma - 2));
                        // Add Player to leaderboard
                        // Only if it was a valid command (not missing , (), or some of the information)
                        if(valid)
                        {
                            std::string result = leaderboard.InsertPlayer(score, player_id);
                            if(result != "")
                            {
                                out_file << result << std::endl;
                            }
                        }
                    }
                    // Removing player
                    else if(line.find("remove_player") != std::string::npos)
                    {
                        int index_parent_first = line.find_first_of("(");
                        int index_parent_last = line.find(")");
                        // Player id is index of first parenth + 1 untill last parathese
                        std::string player_id = line.substr(index_parent_first + 1, (index_parent_last - index_parent_first - 1));
                        // Remove player from leaderboard
                        std::string result = leaderboard.RemovePlayer(player_id);
                        if(result != "")
                        {
                            out_file << result << std::endl;
                        }
                    }
                    // Add score
                    else if(line.find("add_score") != std::string::npos)
                    {
                        // Get score to add and player id
                        int index_parent_first = line.find_first_of("(");
                        int index_comma = line.find(",");
                        int index_parent_last = line.find(")");
                        int score_to_add;
                        bool valid = true;
                        // Score to add is index of first paranthese + 1 untill comma
                        try 
                        {
                            score_to_add = std::stoi(line.substr(index_parent_first + 1, (index_comma - index_parent_first + 1)));
                        }
                        catch(const std::invalid_argument& e)
                        {
                            valid = false;
                        }
                        // Player id is index comma + 2 untill last parathese
                        std::string player_id = line.substr(index_comma + 2, (index_parent_last - index_comma - 2));
                        // Add score to player
                        // Only if valid command
                        if(valid)
                        {
                            std::string result = leaderboard.AddScore(score_to_add, player_id);
                            if(result != "")
                            {
                                out_file << result << std::endl;
                            }
                        }
                    }
                    // Get rank
                    else if(line.find("get_rank") != std::string::npos)
                    {
                        // Get player id
                        int index_parent_first = line.find_first_of("(");
                        int index_parent_last = line.find(")");
                        // Player id is index of first parenth + 1 untill last parathese
                        std::string player_id = line.substr(index_parent_first + 1, (index_parent_last - index_parent_first - 1));  
                        // Get rank of player
                        std::string result = leaderboard.GetRank(player_id);
                        out_file << result << std::endl;       
                    }
                    // Get player with rank
                    else if(line.find("get_player_with_rank") != std::string::npos)
                    {
                        // Get rank k
                        int index_parent_first = line.find_first_of("(");
                        int index_parent_last = line.find(")");
                        int rank;
                        bool valid = true;
                        // Rank k is index of first parenth + 1 untill last parenth
                        try
                        {
                            rank = std::stoi(line.substr(index_parent_first + 1, (index_parent_last - index_parent_first - 1)));
                        }
                        catch(const std::invalid_argument& e)
                        {
                            valid = false;
                        }
                        // Get player with rank 
                        if(valid)
                        {
                            std::string result = leaderboard.GetPlayerWithRank(rank);
                            out_file << result << std::endl;
                        }
                    } 
                    // Get top k players
                    else if(line.find("print_top_k") != std::string::npos)
                    {
                        // Get rank k
                        int index_parent_first = line.find_first_of("(");
                        int index_parent_last = line.find(")");
                        int rank;
                        bool valid = true;
                        try 
                        {
                            // Rank k is index of first parenth + 1 untill last parenth
                            rank = std::stoi(line.substr(index_parent_first + 1, (index_parent_last - index_parent_first - 1)));
                        }
                        catch(const std::invalid_argument& e)
                        {
                            valid = false;
                        }
                        if(valid)
                        {
                            std::string result = leaderboard.PrintTopK(rank);
                            out_file << result;
                        }
                    }
                    else if(line == "quit()")
                    {
                        out_file << "Program has been terminated. Go Gators! \n";
                        // Break to exit reading loops (reading will become false and file will be closed)
                        // For case of quiting before reaching eof
                        break;
                    }
                }
                reading = false;
            }
            file.close();
            out_file.close();
        }
        else
        {
            std::cout << "Couldn't open file" << std::endl;
        }
    }
    return 0;
}