#include "difficulty.h"

DifficultyConfig::DifficultyConfig(Difficulty diff) {
    switch (diff) {
        case EASY:
            mapSize = 5;                   // 5x5 map
            monsterStrengthMultiplier = 0.8f; // Weaker monsters
            trapDamageMultiplier = 0.7f;      // Less damaging traps
            lootQualityMultiplier = 1.2f;     // Better loot
            monsterSpawnRate = 0.3f;          // 30% chance of monster in a room
            trapSpawnRate = 0.2f;             // 20% chance of trap in a room
            lootSpawnRate = 0.5f;             // 50% chance of loot in a room
            break;
            
        case MEDIUM:
            mapSize = 7;                   // 7x7 map
            monsterStrengthMultiplier = 1.0f; // Normal monsters
            trapDamageMultiplier = 1.0f;      // Normal traps
            lootQualityMultiplier = 1.0f;     // Normal loot
            monsterSpawnRate = 0.4f;          // 40% chance of monster in a room
            trapSpawnRate = 0.3f;             // 30% chance of trap in a room
            lootSpawnRate = 0.4f;             // 40% chance of loot in a room
            break;
            
        case HARD:
            mapSize = 10;                   // 10x10 map
            monsterStrengthMultiplier = 1.3f; // Stronger monsters
            trapDamageMultiplier = 1.5f;      // More damaging traps
            lootQualityMultiplier = 0.8f;     // Less quality loot
            monsterSpawnRate = 0.5f;          // 50% chance of monster in a room
            trapSpawnRate = 0.4f;             // 40% chance of trap in a room
            lootSpawnRate = 0.3f;             // 30% chance of loot in a room
            break;
    }
}

int DifficultyConfig::getMapSize() const {
    return mapSize;
}

float DifficultyConfig::getMonsterStrengthMultiplier() const {
    return monsterStrengthMultiplier;
}

float DifficultyConfig::getTrapDamageMultiplier() const {
    return trapDamageMultiplier;
}

float DifficultyConfig::getLootQualityMultiplier() const {
    return lootQualityMultiplier;
}

float DifficultyConfig::getMonsterSpawnRate() const {
    return monsterSpawnRate;
}

float DifficultyConfig::getTrapSpawnRate() const {
    return trapSpawnRate;
}

float DifficultyConfig::getLootSpawnRate() const {
    return lootSpawnRate;
}
