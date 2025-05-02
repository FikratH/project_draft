#include "monster.h"
#include "utils.h"
#include <iostream>

Monster::Monster(const std::string& name, int maxHp, int attack, int defense, int xpReward)
    : Entity(name, maxHp, attack, defense), xpReward(xpReward) {
}

int Monster::getXPReward() const {
    return xpReward;
}

int Monster::calculateLootValue() const {
    // Base loot value based on monster stats
    return (maxHp + attack + defense) / 3;
}

std::vector<int> MonsterFactory::getMonsterStats(MonsterType type) {
    // Returns [maxHp, attack, defense, xpReward]
    switch (type) {
        case GOBLIN:
            return {30, 8, 3, 20};
        case SKELETON:
            return {45, 10, 5, 30};
        case ORC:
            return {60, 12, 8, 40};
        case TROLL:
            return {100, 15, 10, 75};
        case DRAGON:
            return {200, 25, 15, 150};
        default:
            return {20, 5, 2, 10}; // Default weak enemy
    }
}

Monster* MonsterFactory::createMonster(MonsterType type, float difficultyMultiplier) {
    std::string name;
    
    switch (type) {
        case GOBLIN:
            name = "Goblin";
            break;
        case SKELETON:
            name = "Skeleton";
            break;
        case ORC:
            name = "Orc";
            break;
        case TROLL:
            name = "Troll";
            break;
        case DRAGON:
            name = "Dragon";
            break;
        default:
            name = "Unknown";
    }
    
    // Get base stats for the monster type
    std::vector<int> stats = getMonsterStats(type);
    
    // Apply difficulty multiplier
    int maxHp = static_cast<int>(stats[0] * difficultyMultiplier);
    int attack = static_cast<int>(stats[1] * difficultyMultiplier);
    int defense = static_cast<int>(stats[2] * difficultyMultiplier);
    int xpReward = static_cast<int>(stats[3] * difficultyMultiplier);
    
    return new Monster(name, maxHp, attack, defense, xpReward);
}

Monster* MonsterFactory::createRandomMonster(float difficultyMultiplier) {
    // Generate a random monster type based on difficulty
    // Higher difficulty increases chance of stronger monsters
    
    int roll = Utils::getRandomInt(1, 100);
    MonsterType type;
    
    if (roll <= 40) {
        type = GOBLIN;
    } else if (roll <= 70) {
        type = SKELETON;
    } else if (roll <= 85) {
        type = ORC;
    } else if (roll <= 95) {
        type = TROLL;
    } else {
        type = DRAGON;
    }
    
    return createMonster(type, difficultyMultiplier);
}
