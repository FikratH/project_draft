#include "item.h"
#include "player.h"
#include "utils.h"
#include <iostream>
#include <vector>

Item::Item(const std::string& name, ItemType type, int value, const std::string& description)
    : name(name), type(type), value(value), description(description) {
}

std::string Item::getName() const {
    return name;
}

ItemType Item::getType() const {
    return type;
}

int Item::getValue() const {
    return value;
}

std::string Item::getDescription() const {
    return description;
}

void Item::use(Player& player) {
    switch (type) {
        case HEALTH_POTION:
            player.setHP(player.getHP() + value);
            std::cout << "You used a " << name << " and restored " << value << " HP." << std::endl;
            break;
            
        case ATTACK_BOOST:
            player.setAttack(player.getAttack() + value);
            std::cout << "You used a " << name << " and gained " << value << " Attack." << std::endl;
            break;
            
        case DEFENSE_BOOST:
            player.setDefense(player.getDefense() + value);
            std::cout << "You used a " << name << " and gained " << value << " Defense." << std::endl;
            break;
            
        case FULL_HEAL:
            player.setHP(player.getMaxHP());
            std::cout << "You used a " << name << " and fully restored your HP." << std::endl;
            break;
            
        case XP_BOOST:
            player.addXP(value);
            std::cout << "You used a " << name << " and gained " << value << " XP." << std::endl;
            break;
    }
}

void Item::display() const {
    std::cout << name << " - " << description << " (Value: " << value << ")" << std::endl;
}

Item* Item::createRandomItem(float lootQualityMultiplier) {
    // Define possible items
    std::vector<std::pair<std::string, std::string>> healthPotions = {
        {"Small Health Potion", "Restores a small amount of HP"},
        {"Medium Health Potion", "Restores a moderate amount of HP"},
        {"Large Health Potion", "Restores a significant amount of HP"}
    };
    
    std::vector<std::pair<std::string, std::string>> attackBoosts = {
        {"Minor Strength Elixir", "Slightly increases attack power"},
        {"Strength Elixir", "Moderately increases attack power"},
        {"Greater Strength Elixir", "Significantly increases attack power"}
    };
    
    std::vector<std::pair<std::string, std::string>> defenseBoosts = {
        {"Minor Defense Tonic", "Slightly increases defense"},
        {"Defense Tonic", "Moderately increases defense"},
        {"Greater Defense Tonic", "Significantly increases defense"}
    };
    
    std::vector<std::pair<std::string, std::string>> fullHeals = {
        {"Rejuvenation Potion", "Completely restores HP"}
    };
    
    std::vector<std::pair<std::string, std::string>> xpBoosts = {
        {"Minor XP Scroll", "Grants a small amount of XP"},
        {"XP Scroll", "Grants a moderate amount of XP"},
        {"Greater XP Scroll", "Grants a significant amount of XP"}
    };
    
    // Randomly select item type with weighted probabilities
    int randomType = Utils::getRandomInt(1, 100);
    ItemType type;
    
    if (randomType <= 40) {
        type = HEALTH_POTION;
    } else if (randomType <= 60) {
        type = ATTACK_BOOST;
    } else if (randomType <= 80) {
        type = DEFENSE_BOOST;
    } else if (randomType <= 90) {
        type = FULL_HEAL;
    } else {
        type = XP_BOOST;
    }
    
    // Select item tier based on loot quality multiplier
    int tier;
    if (Utils::getRandomFloat(0.0f, 1.0f) < lootQualityMultiplier * 0.3f) {
        tier = 2; // High tier
    } else if (Utils::getRandomFloat(0.0f, 1.0f) < lootQualityMultiplier * 0.6f) {
        tier = 1; // Medium tier
    } else {
        tier = 0; // Low tier
    }
    
    // Determine item properties based on type and tier
    std::string name, description;
    int value;
    
    switch (type) {
        case HEALTH_POTION:
            tier = std::min(tier, static_cast<int>(healthPotions.size() - 1));
            name = healthPotions[tier].first;
            description = healthPotions[tier].second;
            value = static_cast<int>((tier + 1) * 10 * lootQualityMultiplier);
            break;
            
        case ATTACK_BOOST:
            tier = std::min(tier, static_cast<int>(attackBoosts.size() - 1));
            name = attackBoosts[tier].first;
            description = attackBoosts[tier].second;
            value = static_cast<int>((tier + 1) * 2 * lootQualityMultiplier);
            break;
            
        case DEFENSE_BOOST:
            tier = std::min(tier, static_cast<int>(defenseBoosts.size() - 1));
            name = defenseBoosts[tier].first;
            description = defenseBoosts[tier].second;
            value = static_cast<int>((tier + 1) * 2 * lootQualityMultiplier);
            break;
            
        case FULL_HEAL:
            name = fullHeals[0].first;
            description = fullHeals[0].second;
            value = 0; // Not used for full heal
            break;
            
        case XP_BOOST:
            tier = std::min(tier, static_cast<int>(xpBoosts.size() - 1));
            name = xpBoosts[tier].first;
            description = xpBoosts[tier].second;
            value = static_cast<int>((tier + 1) * 25 * lootQualityMultiplier);
            break;
    }
    
    // Create and return the item
    return new Item(name, type, value, description);
}
