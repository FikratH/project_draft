#include "player.h"
#include <iostream>
#include <cmath>

Player::Player(const std::string& name)
    : Entity(name, 100, 10, 5), xp(0), level(1), score(0), gold(0) {
    // Player starts at level 1 with 0 XP and an empty inventory
}

Player::~Player() {
    // No need to explicitly delete inventory, its destructor will be called
}

void Player::addXP(int amount) {
    xp += amount;
    std::cout << "You gained " << amount << " XP!" << std::endl;
    
    // Check for level up
    while (xp >= getXPToNextLevel()) {
        levelUp();
    }
}

int Player::getXP() const {
    return xp;
}

int Player::getLevel() const {
    return level;
}

int Player::getXPToNextLevel() const {
    return calculateXPForLevel(level + 1);
}

void Player::levelUp() {
    level++;
    
    // Improve stats based on new level
    int hpIncrease = 10 + level;
    int atkIncrease = 2 + (level / 3);
    int defIncrease = 1 + (level / 4);
    
    setMaxHP(getMaxHP() + hpIncrease);
    setHP(getMaxHP()); // Full heal on level up
    setAttack(getAttack() + atkIncrease);
    setDefense(getDefense() + defIncrease);
    
    std::cout << "===== LEVEL UP! =====" << std::endl;
    std::cout << "You are now level " << level << "!" << std::endl;
    std::cout << "HP increased by " << hpIncrease << std::endl;
    std::cout << "Attack increased by " << atkIncrease << std::endl;
    std::cout << "Defense increased by " << defIncrease << std::endl;
    std::cout << "=====================" << std::endl;
    
    // Add score for leveling up
    addScore(level * 100);
}

int Player::calculateXPForLevel(int level) const {
    // XP curve: each level requires more XP than the last
    return static_cast<int>(100 * level + 50 * pow(level, 1.5));
}

Inventory& Player::getInventory() {
    return inventory;
}

void Player::useItem() {
    inventory.display();
    
    if (inventory.isEmpty()) {
        return;
    }
    
    std::cout << "Enter the number of the item to use (0 to cancel): ";
    int choice;
    std::cin >> choice;
    
    if (choice <= 0 || choice > inventory.getItemCount()) {
        std::cout << "Cancelled or invalid selection." << std::endl;
        return;
    }
    
    inventory.useItem(choice - 1, *this);
}

void Player::displayStats() const {
    std::cout << "===== " << getName() << " =====" << std::endl;
    std::cout << "Level: " << level << " | XP: " << xp << "/" << getXPToNextLevel() << std::endl;
    std::cout << "HP: " << getHP() << "/" << getMaxHP() << std::endl;
    std::cout << "Attack: " << getAttack() << " | Defense: " << getDefense() << std::endl;
    std::cout << "Score: " << score << std::endl;
    std::cout << "Items: " << inventory.getItemCount() << "/" << inventory.getMaxSize() << std::endl;
    std::cout << "=====================" << std::endl;
}

int Player::getScore() const {
    return score;
}

void Player::addScore(int points) {
    score += points;
    std::cout << "You gained " << points << " points!" << std::endl;
}

int Player::getGold() const {
    return gold;
}

void Player::addGold(int amount) {
    gold += amount;
    if (amount > 0) {
        std::cout << "You gained " << amount << " gold! 💰" << std::endl;
    } else if (amount < 0) {
        std::cout << "You spent " << -amount << " gold." << std::endl;
    }
}

void Player::heal(int amount) {
    int oldHP = getHP();
    int newHP = std::min(getMaxHP(), oldHP + amount);
    setHP(newHP);
    
    int actualHealing = newHP - oldHP;
    
    if (actualHealing > 0) {
        std::cout << "You were healed for " << actualHealing << " HP! ❤️" << std::endl;
    }
}
