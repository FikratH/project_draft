#include "entity.h"
#include <iostream>
#include <algorithm>

Entity::Entity(const std::string& name, int maxHp, int attack, int defense)
    : name(name), maxHp(maxHp), attack(attack), defense(defense) {
    hp = maxHp; // Start with full health
}

std::string Entity::getName() const {
    return name;
}

int Entity::getHP() const {
    return hp;
}

int Entity::getMaxHP() const {
    return maxHp;
}

int Entity::getAttack() const {
    return attack;
}

int Entity::getDefense() const {
    return defense;
}

void Entity::setName(const std::string& name) {
    this->name = name;
}

void Entity::setHP(int hp) {
    this->hp = std::min(hp, maxHp); // HP can't exceed maxHP
}

void Entity::setMaxHP(int maxHp) {
    this->maxHp = maxHp;
    if (hp > maxHp) {
        hp = maxHp; // Adjust current HP if it's now over the new max
    }
}

void Entity::setAttack(int attack) {
    this->attack = attack;
}

void Entity::setDefense(int defense) {
    this->defense = defense;
}

void Entity::takeDamage(int damage) {
    hp = std::max(0, hp - damage); // HP can't go below 0
}

int Entity::calculateDamage(Entity& target) {
    int baseDamage = this->attack - target.getDefense();
    return std::max(1, baseDamage); // Minimum damage is 1
}

bool Entity::isAlive() const {
    return hp > 0;
}

void Entity::displayStats() const {
    std::cout << name << " - HP: " << hp << "/" << maxHp
              << " | ATK: " << attack << " | DEF: " << defense << std::endl;
}
