#include "combat.h"
#include "utils.h"
#include <iostream>
#include <thread>
#include <chrono>

bool Combat::battle(Player& player, Monster& monster) {
    std::cout << "\n===== COMBAT START =====\n";
    std::cout << "You encounter a " << monster.getName() << "!" << std::endl;
    
    bool isPlayerTurn = true; // Player goes first
    //cc
    // Combat loop
    while (player.isAlive() && monster.isAlive()) {
        // Display stats
        std::cout << "\n----- Combat Status -----\n";
        player.displayStats();
        monster.displayStats();
        
        if (isPlayerTurn) {
            playerTurn(player, monster);
        } else {
            monsterTurn(player, monster);
        }
        
        // Switch turns
        isPlayerTurn = !isPlayerTurn;
        
        // Short pause between turns
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout << "\n===== COMBAT END =====\n";
    
    // Return true if player won, false if monster won
    return player.isAlive();
}

int Combat::getCombatAction() {
    std::cout << "\nWhat will you do?\n";
    std::cout << "1. Attack\n";
    std::cout << "2. Use Item\n";
    std::cout << "3. Run Away\n";
    std::cout << "Enter your choice (1-3): ";
    return Utils::getValidInput(1, 3);
}

void Combat::playerTurn(Player& player, Monster& monster) {
    std::cout << "\n----- Your Turn -----\n";
    
    int choice = getCombatAction();
    
    switch (choice) {
        case 1: { // Attack
            int damage = player.calculateDamage(monster);
            std::cout << "You attack the " << monster.getName() << " for " << damage << " damage!" << std::endl;
            monster.takeDamage(damage);
            break;
        }
        case 2: { // Use Item
            player.useItem();
            break;
        }
        case 3: { // Run Away
            // 50% chance to successfully run away
            if (Utils::checkProbability(0.5f)) {
                std::cout << "You successfully escaped from the " << monster.getName() << "!" << std::endl;
                // End combat by reducing monster HP to 0
                monster.takeDamage(monster.getHP());
            } else {
                std::cout << "You failed to escape from the " << monster.getName() << "!" << std::endl;
                // Player loses their turn
            }
            break;
        }
    }
}

void Combat::monsterTurn(Player& player, Monster& monster) {
    std::cout << "\n----- Monster's Turn -----\n";
    
    // Monster always attacks
    int damage = monster.calculateDamage(player);
    std::cout << "The " << monster.getName() << " attacks you for " << damage << " damage!" << std::endl;
    player.takeDamage(damage);
}
