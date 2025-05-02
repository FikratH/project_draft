#ifndef MONSTER_H
#define MONSTER_H

#include "entity.h"
#include "difficulty.h"
#include <vector>
#include <string>

// Enum for different monster types
enum MonsterType {
    GOBLIN,
    SKELETON,
    ORC,
    TROLL,
    DRAGON
};

class Monster : public Entity {
public:
    // Constructor
    Monster(const std::string& name, int maxHp, int attack, int defense, int xpReward);
    
    // Getters
    int getXPReward() const;
    
    // Calculate loot value based on monster strength
    int calculateLootValue() const;
    
private:
    int xpReward;  // XP rewarded when defeated
};

// Monster factory class to create different types of monsters
class MonsterFactory {
public:
    // Create a monster of the specified type and adjusted by difficulty
    static Monster* createMonster(MonsterType type, float difficultyMultiplier);
    
    // Create a random monster based on difficulty
    static Monster* createRandomMonster(float difficultyMultiplier);
    
private:
    // Helper method to get monster stats
    static std::vector<int> getMonsterStats(MonsterType type);
};

#endif // MONSTER_H
