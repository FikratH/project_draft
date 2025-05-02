#ifndef ROOM_H
#define ROOM_H

#include "monster.h"
#include "item.h"
#include "player.h"

// Enum for room types
enum RoomType {
    EMPTY,
    MONSTER_ROOM,
    TRAP_ROOM,
    TREASURE_ROOM,
    ENTRANCE,
    EXIT,
    HEALING_ROOM,     // New: Room with a healing fountain
    CHALLENGE_ROOM,   // New: Room with a special challenge
    MERCHANT_ROOM     // New: Room with a merchant to buy/sell items
};

// Enum for trap types
enum TrapType {
    SPIKE_TRAP,       // Standard spike trap
    POISON_TRAP,      // Poison gas trap
    FALLING_ROCKS,    // Falling rocks from ceiling
    FLAME_TRAP,       // Fire trap
    ICE_TRAP         // Freezing trap
};

// Class to represent a single room in the dungeon
class Room {
public:
    // Constructor
    Room(int x, int y);
    
    // Destructor
    ~Room();
    
    // Getters
    int getX() const;
    int getY() const;
    RoomType getType() const;
    Monster* getMonster() const;
    Item* getTreasure() const;
    int getTrapDamage() const;
    TrapType getTrapType() const;  // Get the type of trap in this room
    bool hasSpecialEvent() const; // Returns true if room has an active special event
    bool isVisited() const;
    bool isNorthOpen() const;
    bool isSouthOpen() const;
    bool isEastOpen() const;
    bool isWestOpen() const;
    
    // Setters
    void setType(RoomType type);
    void setMonster(Monster* monster);
    void setTreasure(Item* treasure);
    void setTrapDamage(int damage);
    void setTrapType(TrapType type);  // Set the type of trap in this room
    void setVisited(bool visited);
    void setNorthOpen(bool open);
    void setSouthOpen(bool open);
    void setEastOpen(bool open);
    void setWestOpen(bool open);
    
    // Enter the room and trigger events
    void enter(Player& player);
    
    // Display room description
    void display() const;
    
    // New helper methods
    std::string trapTypeToString() const; // Convert trap type to string
    void applyRoomEffect(Player& player); // Apply special room effects
    
    // Get character representation for map display
    char getMapChar() const;
    
private:
    int x, y;                // Room coordinates in the dungeon
    RoomType type;           // Type of room
    Monster* monster;        // Monster in the room (if any)
    Item* treasure;          // Treasure in the room (if any)
    int trapDamage;          // Damage dealt by trap (if any)
    TrapType trapType;       // Type of trap in this room
    bool visited;            // Whether the room has been visited
    
    // Walls/doorways
    bool northOpen;
    bool southOpen;
    bool eastOpen;
    bool westOpen;
    
    // Helper functions for different room events
    void monsterEncounter(Player& player);
    void trapEffect(Player& player);
    void treasureFound(Player& player);
};

#endif // ROOM_H
