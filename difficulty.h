#ifndef DIFFICULTY_H
#define DIFFICULTY_H

// Enum to represent different difficulty levels
enum Difficulty {
    EASY,
    MEDIUM,
    HARD
};

// Class to map difficulty levels to game parameters
class DifficultyConfig {
public:
    // Constructor
    DifficultyConfig(Difficulty diff);
    
    // Getters
    int getMapSize() const;
    float getMonsterStrengthMultiplier() const;
    float getTrapDamageMultiplier() const;
    float getLootQualityMultiplier() const;
    float getMonsterSpawnRate() const;
    float getTrapSpawnRate() const;
    float getLootSpawnRate() const;
    
private:
    int mapSize;                      // Size of the dungeon map (NxN)
    float monsterStrengthMultiplier;  // Multiplier for monster stats
    float trapDamageMultiplier;       // Multiplier for trap damage
    float lootQualityMultiplier;      // Multiplier for loot quality
    float monsterSpawnRate;           // Probability of spawning monsters
    float trapSpawnRate;              // Probability of spawning traps
    float lootSpawnRate;              // Probability of spawning loot
};

#endif // DIFFICULTY_H
