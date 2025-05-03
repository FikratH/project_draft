#ifndef DUNGEON_H
#define DUNGEON_H

#include "room.h"
#include "difficulty.h"
#include <vector>

class Dungeon {
public:
    // Constructor
    Dungeon(Difficulty difficulty);
    // Constructor for loading from save (skips maze generation) with a custom map size
    Dungeon(Difficulty difficulty, bool loadFromSave, int mapSize);
    // Constructor for loading from save (skips maze generation)
    Dungeon(Difficulty difficulty, bool loadFromSave);
    
    // Move constructor
    Dungeon(Dungeon&& other) noexcept;
    // Move assignment operator
    Dungeon& operator=(Dungeon&& other) noexcept;
    // Disable copy operations
    Dungeon(const Dungeon&) = delete;
    Dungeon& operator=(const Dungeon&) = delete;

    // Destructor
    ~Dungeon();
    
    // Replace this dungeon's data with another's (move) — used by IO::loadGame
    void replaceWith(Dungeon&& other);
    
    // Initialize the dungeon with rooms
    void initialize();
    
    // Move the player in the specified direction (W, A, S, D)
    // showRoom controls whether to display detailed room information
    bool movePlayer(char direction, Player& player, bool showRoom = true);
    
    // Display the current room
    void displayCurrentRoom() const;
    
    // Display the map
    void displayMap() const;
    
    // Check if the player has reached the exit
    bool hasReachedExit() const;
    
    // Getters
    int getMapSize() const;
    int getCurrentX() const;
    int getCurrentY() const;
    Difficulty getDifficulty() const;
    Room* getRoomAt(int x, int y) const;
    
    // Setters
    void setCurrentPosition(int x, int y);
    
private:
    int mapSize;                // Size of the dungeon map (NxN)
    int currentX, currentY;     // Player's current position
    Difficulty difficulty;      // Difficulty level
    std::vector<std::vector<Room*>> rooms; // 2D grid of rooms
    DifficultyConfig diffConfig;// Difficulty configuration
    
    // Generate the dungeon layout using a maze algorithm
    void generateMaze();
    
    // Populate the dungeon with monsters, traps, and treasures
    void populateRooms();
    
    // Check if coordinates are valid (within the map)
    bool isValidPosition(int x, int y) const;
    
    // Get adjacent rooms that are not yet connected
    std::vector<std::pair<int, int>> getUnconnectedAdjacent(int x, int y) const;
    
    // Connect two rooms by opening walls between them
    void connectRooms(int x1, int y1, int x2, int y2);
};

#endif // DUNGEON_H
