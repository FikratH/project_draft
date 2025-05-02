#ifndef IO_H
#define IO_H

#include "player.h"
#include "dungeon.h"
#include <string>
#include <vector>

struct LeaderboardEntry {
    std::string playerName;
    int score;
    int level;
    std::string date;
    Difficulty difficulty;
};

class IO {
public:
    // Save game state to file
    static bool saveGame(const Player& player, const Dungeon& dungeon);
    
    // List all available save files
    static std::vector<std::string> listSaveFiles();
    
    // Load game state from file
    static bool loadGame(Player& player, Dungeon& dungeon);
    
    // Save player score to leaderboard
    static bool saveToLeaderboard(const Player& player);
    
    // Display leaderboard
    static void displayLeaderboard();
    
private:
    // Constants
    static const std::string SAVE_FILE;
    static const std::string LEADERBOARD_FILE;
    
    // Helper function to get current date and time as string
    static std::string getCurrentDateTime();
    
    // Helper function to read leaderboard entries
    static std::vector<LeaderboardEntry> readLeaderboardEntries();
    
    // Helper function to write leaderboard entries
    static bool writeLeaderboardEntries(const std::vector<LeaderboardEntry>& entries);
    
    // Helper function to serialize a Room
    static std::string serializeRoom(const Room* room);
    
    // Helper function to deserialize a Room
    static void deserializeRoom(Room* room, const std::string& data);
};

#endif // IO_H
