#include "io.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <iomanip>

// Define static constants
const std::string IO::SAVE_FILE = "dungeon_save.dat"; // Default save file name (fallback)
const std::string IO::LEADERBOARD_FILE = "leaderboard.dat";

bool IO::saveGame(const Player& player, const Dungeon& dungeon) {
    // Create save file name based on player's name
    std::string playerName = player.getName();
    // Remove spaces and special characters from name
    playerName.erase(std::remove_if(playerName.begin(), playerName.end(), 
        [](unsigned char c) { return !std::isalnum(c); }), playerName.end());
    
    // Use player name for save file, fallback to default if empty
    std::string saveFileName = playerName.empty() ? SAVE_FILE : (playerName + ".dat");
    
    std::ofstream saveFile(saveFileName, std::ios::out);
    
    if (!saveFile.is_open()) {
        std::cout << "Error: Could not open save file for writing." << std::endl;
        return false;
    }
    
    try {
        // Save player data
        saveFile << "PLAYER\n";
        saveFile << player.getName() << "\n";
        saveFile << player.getHP() << "\n";
        saveFile << player.getMaxHP() << "\n";
        saveFile << player.getAttack() << "\n";
        saveFile << player.getDefense() << "\n";
        saveFile << player.getXP() << "\n";
        saveFile << player.getLevel() << "\n";
        saveFile << player.getScore() << "\n";
        
        // Save inventory
        saveFile << "INVENTORY\n";
        Inventory& inventory = const_cast<Player&>(player).getInventory();
        saveFile << inventory.getItemCount() << "\n";
        
        for (int i = 0; i < inventory.getItemCount(); ++i) {
            Item* item = inventory.getItemAt(i);
            saveFile << item->getName() << "\n";
            saveFile << static_cast<int>(item->getType()) << "\n";
            saveFile << item->getValue() << "\n";
            saveFile << item->getDescription() << "\n";
        }
        
        // Save dungeon data
        saveFile << "DUNGEON\n";
        saveFile << static_cast<int>(dungeon.getDifficulty()) << "\n";
        saveFile << dungeon.getMapSize() << "\n";
        saveFile << dungeon.getCurrentX() << "\n";
        saveFile << dungeon.getCurrentY() << "\n";
        
        // Save room data
        saveFile << "ROOMS\n";
        for (int y = 0; y < dungeon.getMapSize(); ++y) {
            for (int x = 0; x < dungeon.getMapSize(); ++x) {
                Room* room = dungeon.getRoomAt(x, y);
                saveFile << serializeRoom(room) << "\n";
            }
        }
        
        saveFile.close();
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Error during save: " << e.what() << std::endl;
        saveFile.close();
        return false;
    }
}

std::vector<std::string> IO::listSaveFiles() {
    std::vector<std::string> saveFiles;
    
    // Find all .dat files in the current directory that might be save files
    std::string command = "ls -1 *.dat 2>/dev/null || true";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return saveFiles; // Return empty vector if command failed
    }
    
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }
    pclose(pipe);
    
    // Parse the command result into individual file names
    std::istringstream iss(result);
    std::string fileName;
    while (std::getline(iss, fileName)) {
        // Skip the leaderboard file
        if (fileName != LEADERBOARD_FILE) {
            // Remove any trailing whitespace or newlines
            fileName.erase(fileName.find_last_not_of("\n\r ") + 1);
            saveFiles.push_back(fileName);
        }
    }
    
    return saveFiles;
}

bool IO::loadGame(Player& player, Dungeon& dungeon) {
    // List available save files
    std::vector<std::string> saveFiles = listSaveFiles();
    
    if (saveFiles.empty()) {
        std::cout << "No save files found." << std::endl;
        return false;
    }
    
    std::cout << "Available saved games:" << std::endl;
    for (size_t i = 0; i < saveFiles.size(); ++i) {
        // Extract the player name from the file name (remove .dat extension)
        std::string displayName = saveFiles[i];
        size_t dotPos = displayName.find_last_of('.');
        if (dotPos != std::string::npos) {
            displayName = displayName.substr(0, dotPos);
        }
        std::cout << (i + 1) << ": " << displayName << std::endl;
    }
    
    // Ask user to select a save file
    std::cout << "Enter the number of the save file to load (1-" << saveFiles.size() << "): ";
    int choice;
    std::cin >> choice;
    
    // Clear the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Validate choice
    if (choice < 1 || choice > static_cast<int>(saveFiles.size())) {
        std::cout << "Invalid choice." << std::endl;
        return false;
    }
    
    // Load the selected save file
    std::string selectedFile = saveFiles[choice-1];
    std::ifstream loadFile(selectedFile, std::ios::in);
    
    if (!loadFile.is_open()) {
        std::cout << "Error: Could not open save file for reading." << std::endl;
        return false;
    }
    
    try {
        std::string line;
        
        // Load player data
        if (!getline(loadFile, line) || line != "PLAYER") { // PLAYER
            throw std::runtime_error("Invalid save file format: missing PLAYER section");
        }
        
        // Get player name
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing player name");
        }
        std::string playerName = line;
        
        // Read HP
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing player HP");
        }
        int hp = std::stoi(line);
        
        // Read MaxHP
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing player MaxHP");
        }
        int maxHp = std::stoi(line);
        
        // Read Attack
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing player Attack");
        }
        int attack = std::stoi(line);
        
        // Read Defense
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing player Defense");
        }
        int defense = std::stoi(line);
        
        // Read XP
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing player XP");
        }
        int xp = std::stoi(line);
        
        // Read Level
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing player Level");
        }
        int level = std::stoi(line);
        
        // Read Score
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing player Score");
        }
        int score = std::stoi(line);
        
        // Create a new player with the loaded data
        Player newPlayer(playerName);
        newPlayer.setMaxHP(maxHp);
        newPlayer.setHP(hp);
        newPlayer.setAttack(attack);
        newPlayer.setDefense(defense);
        
        // Add XP to reach the correct level
        while (newPlayer.getLevel() < level) {
            newPlayer.addXP(newPlayer.getXPToNextLevel());
        }
        
        // Add remaining XP
        if (xp > newPlayer.getXP()) {
            newPlayer.addXP(xp - newPlayer.getXP());
        }
        
        // Add score
        newPlayer.addScore(score);
        
        // Load inventory
        if (!getline(loadFile, line) || line != "INVENTORY") {
            throw std::runtime_error("Invalid save file format: missing INVENTORY section");
        }
        
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing item count");
        }
        int itemCount = std::stoi(line);
        
        for (int i = 0; i < itemCount; ++i) {
            if (!getline(loadFile, line)) {
                throw std::runtime_error("Invalid save file format: missing item name");
            }
            std::string name = line;
            
            if (!getline(loadFile, line)) {
                throw std::runtime_error("Invalid save file format: missing item type");
            }
            ItemType type = static_cast<ItemType>(std::stoi(line));
            
            if (!getline(loadFile, line)) {
                throw std::runtime_error("Invalid save file format: missing item value");
            }
            int value = std::stoi(line);
            
            if (!getline(loadFile, line)) {
                throw std::runtime_error("Invalid save file format: missing item description");
            }
            std::string description = line;
            
            Item* item = new Item(name, type, value, description);
            newPlayer.getInventory().addItem(item);
        }
        
        // Load dungeon data
        if (!getline(loadFile, line) || line != "DUNGEON") {
            throw std::runtime_error("Invalid save file format: missing DUNGEON section");
        }
        
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing difficulty");
        }
        Difficulty diff = static_cast<Difficulty>(std::stoi(line));
        
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing map size");
        }
        int mapSize = std::stoi(line);
        
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing current X");
        }
        int currentX = std::stoi(line);
        
        if (!getline(loadFile, line)) {
            throw std::runtime_error("Invalid save file format: missing current Y");
        }
        int currentY = std::stoi(line);
        
        // Create a new dungeon with the loaded difficulty
        Dungeon newDungeon(diff);
        
        // Load room data
        if (!getline(loadFile, line) || line != "ROOMS") {
            throw std::runtime_error("Invalid save file format: missing ROOMS section");
        }
        
        for (int y = 0; y < mapSize; ++y) {
            for (int x = 0; x < mapSize; ++x) {
                if (!getline(loadFile, line)) {
                    throw std::runtime_error("Invalid save file format: missing room data");
                }
                Room* room = newDungeon.getRoomAt(x, y);
                deserializeRoom(room, line);
            }
        }
        
        // Set player position
        newDungeon.setCurrentPosition(currentX, currentY);
        
        // Only after everything has loaded successfully, update the reference objects
        player = newPlayer;
        dungeon = newDungeon;
        
        // Update the current room - THIS IS CRITICAL FOR PLAYER MOVEMENT
        dungeon.getRoomAt(dungeon.getCurrentX(), dungeon.getCurrentY())->setVisited(true);
        
        loadFile.close();
        std::cout << "Game loaded successfully!" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Error during load: " << e.what() << std::endl;
        loadFile.close();
        return false;
    }
}

bool IO::saveToLeaderboard(const Player& player) {
    // Read current leaderboard
    std::vector<LeaderboardEntry> entries = readLeaderboardEntries();
    
    // Create new entry
    LeaderboardEntry newEntry;
    newEntry.playerName = player.getName();
    newEntry.score = player.getScore();
    newEntry.level = player.getLevel();
    newEntry.date = getCurrentDateTime();
    newEntry.difficulty = MEDIUM; // Default if not available
    
    // Add new entry
    entries.push_back(newEntry);
    
    // Sort entries by score (descending)
    std::sort(entries.begin(), entries.end(), 
        [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
            return a.score > b.score;
        });
    
    // Limit leaderboard to top 10 entries
    if (entries.size() > 10) {
        entries.resize(10);
    }
    
    // Write updated leaderboard
    return writeLeaderboardEntries(entries);
}

void IO::displayLeaderboard() {
    std::vector<LeaderboardEntry> entries = readLeaderboardEntries();
    
    if (entries.empty()) {
        std::cout << "No leaderboard entries yet." << std::endl;
        return;
    }
    
    std::cout << "\n===== LEADERBOARD =====\n";
    std::cout << std::left << std::setw(5) << "Rank" 
              << std::setw(20) << "Player Name" 
              << std::setw(10) << "Score" 
              << std::setw(10) << "Level" 
              << std::setw(15) << "Difficulty" 
              << "Date\n";
    std::cout << "-------------------------------------------------------------------------\n";
    
    for (size_t i = 0; i < entries.size(); ++i) {
        std::string diffStr;
        switch (entries[i].difficulty) {
            case EASY: diffStr = "Easy"; break;
            case MEDIUM: diffStr = "Medium"; break;
            case HARD: diffStr = "Hard"; break;
            default: diffStr = "Unknown"; break;
        }
        
        std::cout << std::left << std::setw(5) << (i + 1)
                  << std::setw(20) << entries[i].playerName
                  << std::setw(10) << entries[i].score
                  << std::setw(10) << entries[i].level
                  << std::setw(15) << diffStr
                  << entries[i].date << "\n";
    }
    
    std::cout << "=========================\n";
}

std::string IO::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    
    std::tm* local_time = std::localtime(&now_time);
    
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::vector<LeaderboardEntry> IO::readLeaderboardEntries() {
    std::vector<LeaderboardEntry> entries;
    std::ifstream file(LEADERBOARD_FILE, std::ios::in);
    
    if (!file.is_open()) {
        return entries; // Return empty vector if file can't be opened
    }
    
    std::string line;
    while (getline(file, line)) {
        std::vector<std::string> tokens = Utils::split(line, '|');
        
        if (tokens.size() >= 5) {
            LeaderboardEntry entry;
            entry.playerName = tokens[0];
            entry.score = std::stoi(tokens[1]);
            entry.level = std::stoi(tokens[2]);
            entry.difficulty = static_cast<Difficulty>(std::stoi(tokens[3]));
            entry.date = tokens[4];
            entries.push_back(entry);
        }
    }
    
    file.close();
    return entries;
}

bool IO::writeLeaderboardEntries(const std::vector<LeaderboardEntry>& entries) {
    std::ofstream file(LEADERBOARD_FILE, std::ios::out);
    
    if (!file.is_open()) {
        std::cout << "Error: Could not open leaderboard file for writing." << std::endl;
        return false;
    }
    
    for (const auto& entry : entries) {
        file << entry.playerName << "|"
             << entry.score << "|"
             << entry.level << "|"
             << static_cast<int>(entry.difficulty) << "|"
             << entry.date << "\n";
    }
    
    file.close();
    return true;
}

std::string IO::serializeRoom(const Room* room) {
    std::ostringstream oss;
    
    // Room type and properties
    oss << static_cast<int>(room->getType()) << ",";
    oss << (room->isVisited() ? "1" : "0") << ",";
    oss << (room->isNorthOpen() ? "1" : "0") << ",";
    oss << (room->isSouthOpen() ? "1" : "0") << ",";
    oss << (room->isEastOpen() ? "1" : "0") << ",";
    oss << (room->isWestOpen() ? "1" : "0") << ",";
    oss << room->getTrapDamage() << ",";
    
    // Monster data (if exists)
    Monster* monster = room->getMonster();
    if (monster) {
        oss << "1," << monster->getName() << "," 
            << monster->getHP() << "," 
            << monster->getMaxHP() << "," 
            << monster->getAttack() << "," 
            << monster->getDefense() << "," 
            << monster->getXPReward();
    } else {
        oss << "0";
    }
    
    oss << ",";
    
    // Treasure data (if exists)
    Item* treasure = room->getTreasure();
    if (treasure) {
        oss << "1," << treasure->getName() << "," 
            << static_cast<int>(treasure->getType()) << "," 
            << treasure->getValue() << "," 
            << treasure->getDescription();
    } else {
        oss << "0";
    }
    
    return oss.str();
}

void IO::deserializeRoom(Room* room, const std::string& data) {
    std::vector<std::string> parts = Utils::split(data, ',');
    
    if (parts.size() < 8) {
        return; // Not enough data
    }
    
    // Room type and properties
    RoomType type = static_cast<RoomType>(std::stoi(parts[0]));
    bool visited = (parts[1] == "1");
    bool northOpen = (parts[2] == "1");
    bool southOpen = (parts[3] == "1");
    bool eastOpen = (parts[4] == "1");
    bool westOpen = (parts[5] == "1");
    int trapDamage = std::stoi(parts[6]);
    
    room->setType(type);
    room->setVisited(visited);
    room->setNorthOpen(northOpen);
    room->setSouthOpen(southOpen);
    room->setEastOpen(eastOpen);
    room->setWestOpen(westOpen);
    room->setTrapDamage(trapDamage);
    
    // Monster data
    if (parts[7] == "1" && parts.size() >= 14) {
        std::string name = parts[8];
        int hp = std::stoi(parts[9]);
        int maxHp = std::stoi(parts[10]);
        int attack = std::stoi(parts[11]);
        int defense = std::stoi(parts[12]);
        int xpReward = std::stoi(parts[13]);
        
        Monster* monster = new Monster(name, maxHp, attack, defense, xpReward);
        monster->setHP(hp);
        room->setMonster(monster);
        
        // Treasure data
        if (parts.size() >= 15 && parts[14] == "1" && parts.size() >= 19) {
            std::string itemName = parts[15];
            ItemType itemType = static_cast<ItemType>(std::stoi(parts[16]));
            int itemValue = std::stoi(parts[17]);
            std::string itemDesc = parts[18];
            
            Item* item = new Item(itemName, itemType, itemValue, itemDesc);
            room->setTreasure(item);
        }
    } else if (parts.size() >= 9 && parts[8] == "1" && parts.size() >= 13) {
        // No monster, but treasure exists
        std::string itemName = parts[9];
        ItemType itemType = static_cast<ItemType>(std::stoi(parts[10]));
        int itemValue = std::stoi(parts[11]);
        std::string itemDesc = parts[12];
        
        Item* item = new Item(itemName, itemType, itemValue, itemDesc);
        room->setTreasure(item);
    }
}
