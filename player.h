#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "inventory.h"
#include <string>

class Player : public Entity {
public:
    // Constructor
    Player(const std::string& name);
    
    // Destructor
    ~Player();
    
    // XP and leveling
    void addXP(int amount);
    int getXP() const;
    int getLevel() const;
    int getXPToNextLevel() const;
    void levelUp();
    
    // Inventory management
    Inventory& getInventory();
    void useItem();
    
    // Display player stats (overrides Entity::displayStats)
    void displayStats() const override;
    
    // Score tracking
    int getScore() const;
    void addScore(int points);
    
    // Gold handling
    int getGold() const;
    void addGold(int amount);
    
    // Healing functionality
    void heal(int amount);
    
private:
    int xp;             // Current XP
    int level;          // Current level
    Inventory inventory; // Player's inventory
    int score;          // Player's score
    int gold;           // Player's gold
    
    // Calculate XP required for the next level
    int calculateXPForLevel(int level) const;
};

#endif // PLAYER_H
