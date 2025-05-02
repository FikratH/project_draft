#ifndef ENTITY_H
#define ENTITY_H

#include <string>

// Base class for all entities (player and monsters)
class Entity {
public:
    // Constructor
    Entity(const std::string& name, int maxHp, int attack, int defense);
    
    // Virtual destructor
    virtual ~Entity() = default;
    
    // Getters
    std::string getName() const;
    int getHP() const;
    int getMaxHP() const;
    int getAttack() const;
    int getDefense() const;
    
    // Setters
    void setName(const std::string& name);
    void setHP(int hp);
    void setMaxHP(int maxHp);
    void setAttack(int attack);
    void setDefense(int defense);
    
    // Game mechanics
    void takeDamage(int damage);
    int calculateDamage(Entity& target);
    bool isAlive() const;
    
    // Display entity stats
    virtual void displayStats() const;
    
protected:
    std::string name;
    int hp;         // Current health points
    int maxHp;      // Maximum health points
    int attack;     // Attack power
    int defense;    // Defense power
};

#endif // ENTITY_H
