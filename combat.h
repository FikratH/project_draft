#ifndef COMBAT_H
#define COMBAT_H

#include "entity.h"
#include "player.h"
#include "monster.h"

class Combat {
public:
    // Main battle function to handle combat between a player and a monster
    static bool battle(Player& player, Monster& monster);
    
private:
    // Helper function to display combat menu and get player choice
    static int getCombatAction();
    
    // Helper function to execute a player turn
    static void playerTurn(Player& player, Monster& monster);
    
    // Helper function to execute a monster turn
    static void monsterTurn(Player& player, Monster& monster);
};

#endif // COMBAT_H
