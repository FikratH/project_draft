#ifndef ITEM_H
#define ITEM_H

#include <string>

// Forward declaration
class Player;

// Enum to represent different item types
enum ItemType {
    HEALTH_POTION,
    ATTACK_BOOST,
    DEFENSE_BOOST,
    FULL_HEAL,
    XP_BOOST,
    CONSUMABLE    // Generic consumable item type
};

class Item {
public:
    // Constructor
    Item(const std::string& name, ItemType type, int value, const std::string& description);
    
    // Virtual destructor
    virtual ~Item() = default;
    
    // Getters
    std::string getName() const;
    ItemType getType() const;
    int getValue() const;
    std::string getDescription() const;
    
    // Apply the item's effect to the player
    virtual void use(Player& player);
    
    // Display item information
    void display() const;
    
    // Static function to create a random item based on loot quality
    static Item* createRandomItem(float lootQualityMultiplier);
    
private:
    std::string name;
    ItemType type;
    int value;              // The power/amount of the item's effect
    std::string description;
};

#endif // ITEM_H
