#include "player.h"
#include "utils.h"
#include <iostream>
#include <cmath>

Player::Player(const std::string &name)
    : Entity(name, 100, 10, 5), xp(0), level(1), score(0), gold(0)
{
    // Player starts at level 1 with 0 XP and an empty inventory
}

Player::~Player()
{
    // No need to explicitly delete inventory, its destructor will be called
}

void Player::addXP(int amount)
{
    xp += amount;
    Utils::slowPrint("You gained " + std::to_string(amount) + " XP!\n", 30);

    // Check for level up
    while (xp >= getXPToNextLevel())
    {
        levelUp();
    }
}

int Player::getXP() const
{
    return xp;
}

int Player::getLevel() const
{
    return level;
}

int Player::getXPToNextLevel() const
{
    return calculateXPForLevel(level + 1);
}

void Player::levelUp()
{
    level++;

    // Improve stats based on new level
    int hpIncrease = 10 + level;
    int atkIncrease = 2 + (level / 3);
    int defIncrease = 1 + (level / 4);

    setMaxHP(getMaxHP() + hpIncrease);
    setHP(getMaxHP()); // Full heal on level up
    setAttack(getAttack() + atkIncrease);
    setDefense(getDefense() + defIncrease);

    std::string levelUpText =
        "===== LEVEL UP! =====\n You are now level " + std::to_string(level) + "!\n" +
        "HP increased by " + std::to_string(hpIncrease) + "\n" +
        "Attack increased by " + std::to_string(atkIncrease) + "\n" +
        "Defense increased by " + std::to_string(defIncrease) + "\n" +
        "=====================\n";

    Utils::slowVerticalPrint(levelUpText, 150);

    // Add score for leveling up
    addScore(level * 100);
}

int Player::calculateXPForLevel(int level) const
{
    // XP curve: each level requires more XP than the last
    return static_cast<int>(100 * level + 50 * pow(level, 1.5));
}

Inventory &Player::getInventory()
{
    return inventory;
}

void Player::useItem()
{
    inventory.display();

    if (inventory.isEmpty())
    {
        return;
    }

    Utils::slowPrint("Enter the number of the item to use (0 to cancel): ", 30);
    int choice;
    std::cin >> choice;

    if (choice <= 0 || choice > inventory.getItemCount())
    {
        std::cout << "Cancelled or invalid selection." << std::endl;
        return;
    }

    inventory.useItem(choice - 1, *this);
}

void Player::displayStats() const
{
    std::string stats =
        "===== " + getName() + " =====\n" +
        "Level: " + std::to_string(level) + " | XP: " + std::to_string(xp) + "/" + std::to_string(getXPToNextLevel()) + "\n" +
        "HP: " + std::to_string(getHP()) + "/" + std::to_string(getMaxHP()) + "\n" +
        "Attack: " + std::to_string(getAttack()) + " | Defense: " + std::to_string(getDefense()) + "\n" +
        "Score: " + std::to_string(score) + "\n" +
        "Items: " + std::to_string(inventory.getItemCount()) + "/" + std::to_string(inventory.getMaxSize()) + "\n" +
        "=====================\n";

    Utils::slowVerticalPrint(stats, 150);
}

int Player::getScore() const
{
    return score;
}

void Player::addScore(int points)
{
    score += points;
    Utils::slowPrint("You gained " + std::to_string(points) + " points!\n", 30);
}

int Player::getGold() const
{
    return gold;
}

void Player::addGold(int amount)
{
    gold += amount;

    if (amount > 0)
    {
        Utils::slowPrint("You gained " + std::to_string(amount) + " gold! 💰\n", 30);
    }
    else if (amount < 0)
    {
        Utils::slowPrint("You spent " + std::to_string(-amount) + " gold.\n", 30);
    }
}

void Player::heal(int amount)
{
    int oldHP = getHP();
    int newHP = std::min(getMaxHP(), oldHP + amount);
    setHP(newHP);

    int actualHealing = newHP - oldHP;

    if (actualHealing > 0)
    {
        Utils::slowPrint("You were healed for " + std::to_string(actualHealing) + " HP! ❤️\n", 30);
    }
}
