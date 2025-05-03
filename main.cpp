#include <iostream>
#include <string>
#include <thread>
#include <limits>
#include "utils.h"
#include "difficulty.h"
#include "player.h"
#include "dungeon.h"
#include "io.h"

// Just the essentials
void startGameLoop(Player& player, Dungeon& dungeon);
void startGame(Difficulty difficulty);

int main() {
    // Splash screen - gotta look cool, right?
    std::cout << "=================================\n";
    std::cout << "   DUNGEON CRAWLER ADVENTURE    \n";
    std::cout << "=================================\n\n";
    std::cout << "Welcome brave adventurer! Prepare to delve into the depths\n";
    std::cout << "of a dangerous dungeon filled with monsters, traps, and treasures.\n\n";

    bool isPlaying = true;
    int choice;

    while (isPlaying) {
        // Let's see what the player wants to do
        std::cout << "MAIN MENU:\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. View Leaderboard\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice (1-4): ";
        choice = Utils::getValidInput(1, 4);

        switch (choice) {
            case 1: { // Let's start fresh
                Utils::slowPrint("\nSelect difficulty:\n", 15);
                Utils::slowPrint("1. Easy (5x5 map, weaker monsters)\n", 15);
                Utils::slowPrint("2. Medium (7x7 map, balanced experience)\n", 15);
                Utils::slowPrint("3. Hard (10x10 map, stronger monsters)\n", 15);
                Utils::slowPrint("Enter difficulty (1-3): ", 15);
                int diffChoice = Utils::getValidInput(1, 3);
                Difficulty diff = static_cast<Difficulty>(diffChoice - 1);
                
                startGame(diff);
                
                // TODO: Save score to leaderboard when we're done
                // IO::saveToLeaderboard(player);
                break;
            }
            case 2: { // Pick up where we left off
                Player player("");
                Dungeon dungeon(EASY); // Placeholder until we load the real save
                if (IO::loadGame(player, dungeon)) {
                    std::cout << "Game loaded successfully!\n";
                    startGameLoop(player, dungeon);
                    IO::saveToLeaderboard(player, dungeon);
                } else {
                    std::cout << "Failed to load game. Returning to main menu.\n";
                }
                break;
            }
            case 3: // Let's check the high scores
                IO::displayLeaderboard();
                break;
            case 4: // Time to call it a day
                std::cout << "Thanks for playing! Goodbye.\n";
                isPlaying = false;
                break;
        }
    }
    
    return 0;
}

// Get this adventure started
void startGame(Difficulty difficulty) {
    // Who do you want to be today?
    std::string name;
    std::cout << "\nEnter your character's name: ";
    std::getline(std::cin, name);
    
    Player player(name);
    
    // Tune the player stats for the chosen difficulty
    switch (difficulty) {
        case EASY: // Novice mode - more health, better stats
            player.setMaxHP(120);
            player.setHP(120);
            player.setAttack(25);
            player.setDefense(15);
            break;
        case MEDIUM: // Balanced
            player.setMaxHP(100);
            player.setHP(100);
            player.setAttack(20);
            player.setDefense(10);
            break;
        case HARD: // For the brave (or foolish)
            player.setMaxHP(80);
            player.setHP(80);
            player.setAttack(15);
            player.setDefense(5);
            break;
    }
    
    // Initialize dungeon
    Dungeon dungeon(difficulty);
    
    // Clear screen before starting the game
    Utils::clearScreen();
    
    // Display entrance message
    std::cout << "\n💬 " << player.getName() << " has entered the mysterious cave... there is no way back.\n";
    std::cout << "💬 You must find the exit to escape!\n\n";
    
    // Make sure the entrance room is visited so it's visible on the map
    dungeon.getRoomAt(dungeon.getCurrentX(), dungeon.getCurrentY())->setVisited(true);

    // Run game loop
    startGameLoop(player, dungeon);
    
    // After game ends, save score to leaderboard
    IO::saveToLeaderboard(player, dungeon);
}

// Game loop implementation
void startGameLoop(Player& player, Dungeon& dungeon) {
    bool isRunning = true;
    Utils::clearScreen();
    std::cout << "\n╔════════════════════════════╗\n";
    std::cout << "║    THE ADVENTURE BEGINS!   ║\n";
    std::cout << "╚════════════════════════════╝\n";
    std::cout << "You enter the dungeon...\n";
    
    // Give the player a moment to read the intro
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Show initial instructions
    Utils::clearScreen();
    std::cout << "\n╔════════ CONTROLS ═════════╗\n";
    std::cout << "║ W - Move North            ║\n";
    std::cout << "║ A - Move West             ║\n";
    std::cout << "║ S - Move South            ║\n";
    std::cout << "║ D - Move East             ║\n";
    std::cout << "║ I - Use Item              ║\n";
    std::cout << "║ P - Player Stats          ║\n";
    std::cout << "║ V - Save Game             ║\n";
    std::cout << "║ Q - Quit to Main Menu     ║\n";
    std::cout << "╚═══════════════════════════╝\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Always display the dungeon map at the beginning
    dungeon.displayMap();
    
    // Display player info and controls function
    auto displayUIElements = [&player]() {
        // Display a small summary of player status
        std::cout << "\n╔════════ PLAYER ════════╗\n";
        std::cout << "║ Name: " << player.getName();
        for (unsigned int i = 0; i < 17 - player.getName().length(); i++) std::cout << " ";
        std::cout << "║\n";
        std::cout << "║ HP: " << player.getHP() << "/" << player.getMaxHP();
        int hpLen = std::to_string(player.getHP()).length() + std::to_string(player.getMaxHP()).length() + 1;
        for (int i = 0; i < 18 - hpLen; i++) std::cout << " ";
        std::cout << " ║\n";
        std::cout << "╚════════════════════════╝\n";
        
        // Always show controls
        std::cout << "\n╔════════════ CONTROLS ═══════════╗\n";
        std::cout << "║ WASD: Move | I: Item | P: Stats ║\n";
        std::cout << "║ V: Save | Q: Quit               ║\n";
        std::cout << "╚═════════════════════════════════╝\n";
    };
    
    // Display player info and controls
    displayUIElements();
    
    while (isRunning) {
        // Show available exits of current room
        Room* currentRoom = dungeon.getRoomAt(dungeon.getCurrentX(), dungeon.getCurrentY());
        std::cout << "\nAvailable exits: ";
        if (currentRoom->isNorthOpen()) std::cout << "North(W) ";
        if (currentRoom->isSouthOpen()) std::cout << "South(S) ";
        if (currentRoom->isEastOpen()) std::cout << "East(D) ";
        if (currentRoom->isWestOpen()) std::cout << "West(A) ";
        std::cout << "\n";
        
        std::cout << "\nEnter command: ";
        char input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer
        input = toupper(input);
        
        // Define bool moved variable outside the switch
        bool moved;
        
        switch (input) {
            case 'W': // Move North
            case 'A': // Move West
            case 'S': // Move South
            case 'D': // Move East
                // Try to move player
                moved = dungeon.movePlayer(input, player, false); // false = don't display room details
                if (!moved) {
                    std::cout << "\n\033[1;31mYou cannot go that way!\033[0m Press Enter to continue...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    // After movement, always refresh map
                    Utils::clearScreen();
                    dungeon.displayMap();
                    
                    // Display UI elements
                    displayUIElements();
                }
                break;
                
            case 'I': // Use Item
                Utils::clearScreen();
                player.useItem();
                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                // Refresh dungeon map after using item
                Utils::clearScreen();
                dungeon.displayMap();
                displayUIElements();
                break;
                
            case 'P': // Player Stats
                Utils::clearScreen();
                std::cout << "\n╔═════ PLAYER STATS ═════╗\n";
                player.displayStats();
                std::cout << "╚════════════════════════╝\n";
                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                // Refresh dungeon map after viewing stats
                Utils::clearScreen();
                dungeon.displayMap();
                displayUIElements();
                break;
                
            case 'V': // Save Game
                Utils::clearScreen();
                std::cout << "\n╔════════ SAVING GAME ════════╗\n";
                if (IO::saveGame(player, dungeon)) {
                    std::cout << "║   Game saved successfully!  ║\n";
                } else {
                    std::cout << "║     Failed to save game!    ║\n";
                }
                std::cout << "╚═════════════════════════════╝\n";
                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                // Refresh dungeon map after saving
                Utils::clearScreen();
                dungeon.displayMap();
                displayUIElements();
                break;
                
            case 'Q': // Quit to Main Menu
                isRunning = false;
                break;
                
            default:
                std::cout << "\n\033[1;31mInvalid command!\033[0m Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
        
        // Check if player is dead
        if (player.getHP() <= 0) {
            Utils::clearScreen();
            std::cout << "\n╔════════ GAME OVER ════════╗\n";
            std::cout << "║  You have been defeated!  ║\n";
            std::cout << "╚═══════════════════════════╝\n";
            std::cout << "Final Score: " << player.getScore() << "\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            isRunning = false;
        }
        
        // Check if player has reached the exit
        if (dungeon.hasReachedExit()) {
            Utils::clearScreen();
            std::cout << "\n╔═════════ VICTORY ═════════╗\n";
            std::cout << "║ Congratulations!          ║\n";
            std::cout << "║ You escaped the dungeon!  ║\n";
            std::cout << "╚═══════════════════════════╝\n";
            player.addScore(1000); // Bonus for completing
            std::cout << "Final Score: " << player.getScore() << "\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            isRunning = false;
        }
    }
}
