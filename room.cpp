#include "room.h"
#include "combat.h"
#include "utils.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <cstdlib>

Room::Room(int x, int y)
    : x(x), y(y), type(EMPTY), monster(nullptr), treasure(nullptr),
      trapDamage(0), trapType(SPIKE_TRAP), visited(false), northOpen(false), southOpen(false),
      eastOpen(false), westOpen(false)
{
}

Room::~Room()
{
    // Don't leave monsters and treasures lying around in memory
    if (monster != nullptr)
    {
        delete monster;
    }
    if (treasure != nullptr)
    {
        delete treasure;
    }
}

int Room::getX() const
{
    return x;
}

int Room::getY() const
{
    return y;
}

RoomType Room::getType() const
{
    return type;
}

Monster *Room::getMonster() const
{
    return monster;
}

Item *Room::getTreasure() const
{
    return treasure;
}

int Room::getTrapDamage() const
{
    return trapDamage;
}

TrapType Room::getTrapType() const
{
    return trapType;
}

bool Room::hasSpecialEvent() const
{
    // Check if room has an active special event
    switch (type)
    {
    case MONSTER_ROOM:
        return (monster && monster->isAlive());
    case TRAP_ROOM:
        return !visited; // Only first visit triggers trap
    case TREASURE_ROOM:
        return (treasure != nullptr);
    case EXIT:
        return true; // Exit is always special
    case HEALING_ROOM:
        return true; // Healing fountain is special
    case CHALLENGE_ROOM:
        return true; // Challenge is special
    case MERCHANT_ROOM:
        return true; // Merchant is special
    default:
        return false;
    }
}

bool Room::isVisited() const
{
    return visited;
}

bool Room::isNorthOpen() const
{
    return northOpen;
}

bool Room::isSouthOpen() const
{
    return southOpen;
}

bool Room::isEastOpen() const
{
    return eastOpen;
}

bool Room::isWestOpen() const
{
    return westOpen;
}

void Room::setType(RoomType type)
{
    this->type = type;
}

void Room::setMonster(Monster *monster)
{
    this->monster = monster;
}

void Room::setTreasure(Item *treasure)
{
    this->treasure = treasure;
}

void Room::setTrapDamage(int damage)
{
    trapDamage = damage;
}

void Room::setTrapType(TrapType type)
{
    trapType = type;
}

std::string Room::trapTypeToString() const
{
    TrapType currentTrapType = trapType;
    switch (currentTrapType)
    {
    case SPIKE_TRAP:
        return "spike";
    case POISON_TRAP:
        return "poison";
    case FALLING_ROCKS:
        return "falling rock";
    case FLAME_TRAP:
        return "flame";
    case ICE_TRAP:
        return "freezing";
    default:
        return "unknown";
    }
}

void Room::setVisited(bool visited)
{
    this->visited = visited;
}

void Room::setNorthOpen(bool open)
{
    this->northOpen = open;
}

void Room::setSouthOpen(bool open)
{
    this->southOpen = open;
}

void Room::setEastOpen(bool open)
{
    this->eastOpen = open;
}

void Room::setWestOpen(bool open)
{
    this->westOpen = open;
}

void Room::enter(Player &player)
{
    // Mark room as visited
    visited = true;

    // Display room description
    display();

    // Show announcement panel for special rooms
    if (type != EMPTY && !((type == MONSTER_ROOM && monster && !monster->isAlive())))
    {
        std::cout << "\n";
        // Display a highlighted banner based on room type
        // Animations added
        switch (type)
        {
        case MONSTER_ROOM:
            if (monster && monster->isAlive())
            {
                Utils::slowVerticalPrint("\033[1;31m╔═════════ CHALLENGE ═════════╗\033[0m\n", 150);
                Utils::slowVerticalPrint("\033[1;31m║      MONSTER ENCOUNTER!     ║\033[0m\n", 150);
                Utils::slowVerticalPrint("\033[1;31m╚═════════════════════════════╝\033[0m\n", 150);
                Utils::slowVerticalPrint("\nA " + monster->getName() + " stands in your way! Prepare for battle!\n", 150);
            }
            break;

        case TRAP_ROOM:
            Utils::slowVerticalPrint("\033[1;33m╔═════════ CHALLENGE ═════════╗\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;33m║         DEADLY TRAP!        ║\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;33m╚═════════════════════════════╝\033[0m\n", 150);
            break;

        case TREASURE_ROOM:
            if (treasure)
            {
                Utils::slowVerticalPrint("\033[1;32m╔══════════ REWARD ══════════╗\033[0m\n", 150);
                Utils::slowVerticalPrint("\033[1;32m║       TREASURE FOUND!      ║\033[0m\n", 150);
                Utils::slowVerticalPrint("\033[1;32m╚════════════════════════════╝\033[0m\n", 150);
            }
            break;

        case EXIT:
            Utils::slowVerticalPrint("\033[1;36m╔══════════ GOAL ══════════╗\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;36m║        EXIT FOUND!       ║\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;36m╚══════════════════════════╝\033[0m\n", 150);
            break;

        case HEALING_ROOM:
            Utils::slowVerticalPrint("\033[1;32m╔═════════ SANCTUARY ═════════╗\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;32m║   ♡   HEALING FOUNTAIN   ♡  ║\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;32m╚═════════════════════════════╝\033[0m\n", 150);

            break;

        case CHALLENGE_ROOM:
            Utils::slowVerticalPrint("\033[1;35m╔═════════ CHALLENGE ═════════╗\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;35m║      TEST YOUR SKILLS       ║\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;35m╚═════════════════════════════╝\033[0m\n", 150);
            break;

        case MERCHANT_ROOM:
            Utils::slowVerticalPrint("\033[1;33m╔══════════ TRADER ══════════╗\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;33m║    $   MERCHANT SHOP   $   ║\033[0m\n", 150);
            Utils::slowVerticalPrint("\033[1;33m╚════════════════════════════╝\033[0m\n", 150);
            break;

        case ENTRANCE:
            // No special banner here; entrance handled separately
            break;

        case EMPTY:
            // Never announce empty rooms
            break;
        }
    }

    // Add prompt to press Enter to continue
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Trigger room event based on type
    switch (type)
    {
    case MONSTER_ROOM:
        monsterEncounter(player);
        break;

    case TRAP_ROOM:
        trapEffect(player);
        break;

    case TREASURE_ROOM:
        treasureFound(player);
        break;

    case ENTRANCE:
        Utils::slowPrint("This is the entrance to the dungeon.", 20);
        break;

    case EXIT:
        Utils::slowPrint("You found the exit! You can now leave the dungeon.", 20);
        break;

    case HEALING_ROOM:
    case CHALLENGE_ROOM:
    case MERCHANT_ROOM:
        // Apply the effect for the new room types
        applyRoomEffect(player);
        break;

    case EMPTY:
    default:
        std::cout << "This room is empty." << std::endl;
        break;
    }
}

void Room::display() const
{
    Utils::clearScreen();

    // Get a more descriptive room name based on the room type
    std::string roomName = "ROOM";
    std::string roomEmoji = "";

    switch (type)
    {
    case MONSTER_ROOM:
        roomName = monster && monster->isAlive() ? "BATTLE" : "CLEARED";
        roomEmoji = monster && monster->isAlive() ? "⚔️ " : "💀 ";
        break;
    case TRAP_ROOM:
        roomName = "DANGER";
        roomEmoji = "⚠️ ";
        break;
    case TREASURE_ROOM:
        roomName = treasure ? "TREASURE" : "LOOTED";
        roomEmoji = treasure ? "💰 " : "📦 ";
        break;
    case ENTRANCE:
        roomName = "ENTRANCE";
        roomEmoji = "🏠 ";
        break;
    case EXIT:
        roomName = "EXIT";
        roomEmoji = "🚪 ";
        break;
    default:
        roomName = "EMPTY";
        roomEmoji = "";
    }

    // ——— Fixed-width banner per room type ———
    std::cout << "\n";
    switch (type)
    {
    case MONSTER_ROOM:
        if (monster && monster->isAlive())
        {
            std::string battleBanner =
                "\033[1;36m╔═══════════⚔️ BATTLE [" + std::to_string(x) + "," + std::to_string(y) + "]═════════════╗\033[0m\n";

            Utils::slowVerticalPrint(battleBanner, 200);
        }
        else
        {
            std::string clearedBanner =
                "\033[1;36m╔═══════════💀 CLEARED [" + std::to_string(x) + "," + std::to_string(y) + "]════════════╗\033[0m\n";

            Utils::slowVerticalPrint(clearedBanner, 200);
        }
        break;
            case TRAP_ROOM: {
                std::string msg = "\033[1;36m╔════════════⚠️ DANGER [" + std::to_string(x) + "," + std::to_string(y) + "]════════════╗\033[0m\n";
                Utils::slowVerticalPrint(msg, 200);
                break;
            }
            case TREASURE_ROOM: {
                std::string msg = "\033[1;36m╔═══════════💰 TREASURE [" + std::to_string(x) + "," + std::to_string(y) + "]══════════╗\033[0m\n";
                Utils::slowVerticalPrint(msg, 200);
                break;
            }
            case ENTRANCE: {
                std::string msg = "\033[1;36m╔═══════════🏠 ENTRANCE [" + std::to_string(x) + "," + std::to_string(y) + "]═══════════╗\033[0m\n";
                Utils::slowVerticalPrint(msg, 200);
                break;
            }
            case EXIT: {
                std::string msg = "\033[1;36m╔═════════════🚪 EXIT [" + std::to_string(x) + "," + std::to_string(y) + "]════════════╗\033[0m\n";
                Utils::slowVerticalPrint(msg, 200);
                break;
            }
            case HEALING_ROOM: {
                std::string msg = "\033[1;36m╔═════════════💚 HEAL [" + std::to_string(x) + "," + std::to_string(y) + "]════════════╗\033[0m\n";
                Utils::slowVerticalPrint(msg, 200);
                break;
            }
            case CHALLENGE_ROOM: {
                std::string msg = "\033[1;36m╔══════════🎯 CHALLENGE [" + std::to_string(x) + "," + std::to_string(y) + "]══════════╗\033[0m\n";
                Utils::slowVerticalPrint(msg, 200);
                break;
            }
            case MERCHANT_ROOM: {
                std::string msg = "\033[1;36m╔════════════💵 SHOP [" + std::to_string(x) + "," + std::to_string(y) + "]═════════════╗\033[0m\n";
                Utils::slowVerticalPrint(msg, 200);
                break;
            }
            default: {
                std::string msg = "\033[1;36m╔═════════════ EMPTY [" + std::to_string(x) + "," + std::to_string(y) + "]═════════════╗\033[0m\n";
                Utils::slowVerticalPrint(msg, 200);
                break;
            
        }
        
        break;
    }
    // ————————————————————————————————

    // Top border with north exit
    if (northOpen)
    {
        std::cout << "\033[1;36m║\033[0m                  \033[1;33m↑\033[0m                   \033[1;36m║\033[0m\n";
    }
    else
    {
        std::cout << "\033[1;36m╠══════════════════════════════════════╣\033[0m\n";
    }

    // Room interior with west and east exits
    if (westOpen)
    {
        std::cout << "\033[1;33m←\033[0m ";
    }
    else
    {
        std::cout << "\033[1;36m║\033[0m ";
    }

    // Room contents based on type - now more descriptive and colorful
    switch (type)
    {
    case MONSTER_ROOM:
        if (monster && monster->isAlive())
        {
            Utils::slowVerticalPrint("\033[1;31m👹 MONSTER\033[0m       \033[1;32mΩ\033[0m", 150);
        }
        else
        {
            Utils::slowVerticalPrint("\033[1;90m💀 DEFEATED\033[0m      \033[1;32mΩ\033[0m", 150);
        }
        break;
    case TRAP_ROOM:
        Utils::slowVerticalPrint("\033[1;33m⚠️ TRAP\033[0m            \033[1;32mΩ\033[0m", 150);
        break;
    case TREASURE_ROOM:
        if (treasure)
        {
            Utils::slowVerticalPrint("\033[1;33m💰 GOLD\033[0m          \033[1;32mΩ\033[0m", 150);
        }
        else
        {
            Utils::slowVerticalPrint("\033[1;90m📦 EMPTY\033[0m         \033[1;32mΩ\033[0m", 150);
        }
        break;
    case ENTRANCE:
        Utils::slowVerticalPrint("\033[1;32m🏠 START\033[0m         \033[1;32mΩ\033[0m", 150);
        break;
    case EXIT:
        Utils::slowVerticalPrint("\033[1;36m🚪 EXIT\033[0m          \033[1;32mΩ\033[0m", 150);
        break;
    default:
    Utils::slowVerticalPrint("                 \033[1;32mΩ\033[0m", 150);
    }

    if (eastOpen)
    {
        Utils::slowVerticalPrint("                   \033[1;33m→\033[0m\n", 150);
    }
    else
    {
        Utils::slowVerticalPrint(" \033[1;36m                  ║\033[0m\n", 150);
    }

    // Bottom border with south exit
    if (southOpen)
    {
        Utils::slowVerticalPrint("\033[1;36m║\033[0m                  \033[1;33m↓\033[0m                   \033[1;36m║\033[0m\n", 150);
    }
    else
    {
        Utils::slowVerticalPrint("\033[1;36m╠══════════════════════════════════════╣\033[0m\n", 150);
    }

    // Room description section
    Utils::slowVerticalPrint("\033[1;36m║\033[0m                                      \033[1;36m║\033[0m\n", 150);

    // Show room type with more descriptive text
    switch (type)
    {
    case MONSTER_ROOM:
        if (monster && monster->isAlive())
        {
            std::string monsterName = monster->getName();
int monsterNameSpace = std::max(0, 35 - static_cast<int>(monsterName.length()));
std::string spacing(monsterNameSpace, ' ');

std::string monsterIntro =
    "\033[1;36m║\033[0m\033[1;31m A fearsome monster stands before you!\033[0m\033[1;36m║\033[0m\n"
    "\033[1;36m║\033[0m\033[1;31m Prepare for battle!                  \033[0m\033[1;36m║\033[0m\n"
    "\033[1;36m║\033[0m \033[1;31mA " + monsterName + "\033[0m" + spacing + "\033[1;36m║\033[0m\n"
    "\033[1;36m║\033[0m \033[1;31mIt looks dangerous!\033[0m                  \033[1;36m║\033[0m\n"
    "\033[1;36m║\033[0m \033[1;31mPrepare for battle...\033[0m                \033[1;36m║\033[0m\n";

Utils::slowVerticalPrint(monsterIntro, 150);

        }
        else
        {
            std::string monsterName = monster->getName();
int monsterNameSpace = std::max(0, 36 - static_cast<int>(monsterName.length()));
std::string spacing(monsterNameSpace, ' ');

std::string defeatedText =
    "\033[1;36m║\033[0m \033[1;90mYou've defeated the\033[0m                  \033[1;36m║\033[0m\n"
    "\033[1;36m║\033[0m \033[1;90m" + monsterName + "\033[0m" + spacing + "\033[1;36m║\033[0m\n"
    "\033[1;36m║\033[0m \033[1;90mWell done, adventurer!\033[0m               \033[1;36m║\033[0m\n";

Utils::slowVerticalPrint(defeatedText, 150);

        }
        break;

    case TRAP_ROOM:
        if (!visited)
        {
            std::cout << "\033[1;36m║\033[0m \033[1;33mThis room feels dangerous...\033[0m          \033[1;36m║\033[0m\n";
            std::cout << "\033[1;36m║\033[0m \033[1;33mBe on your guard!\033[0m                     \033[1;36m║\033[0m\n";
            std::cout << "\033[1;36m║\033[0m \033[1;33mReact quickly if needed!\033[0m              \033[1;36m║\033[0m\n";
        }
        else
        {
            std::cout << "\033[1;36m║\033[0m \033[1;33mA " << trapTypeToString() << " trap!\033[0m";
            int trapNameSpace = std::max(0, 29 - static_cast<int>(trapTypeToString().length()));
            for (int i = 0; i < trapNameSpace; i++)
                std::cout << " ";
            std::cout << "\033[1;36m║\033[0m\n";
            std::cout << "\033[1;36m║\033[0m \033[1;33mIt deals " << trapDamage << " damage\033[0m";
            int dmgSpace = std::max(0, 13 - static_cast<int>(std::to_string(trapDamage).length()));
            for (int i = 0; i < dmgSpace; i++)
                std::cout << " ";
            std::cout << "\033[1;36m        ║\033[0m\n";
            std::cout << "\033[1;36m║\033[0m \033[1;33mWatch your step!\033[0m                     \033[1;36m║\033[0m\n";
        }
        break;

    case TREASURE_ROOM:
        if (treasure)
        {
            std::cout << "\033[1;36m║\033[0m \033[1;33mA magnificent treasure!\033[0m              \033[1;36m║\033[0m\n";
            std::cout << "\033[1;36m║\033[0m \033[1;33mThe chest shimmers with gold\033[0m         \033[1;36m║\033[0m\n";
            std::cout << "\033[1;36m║\033[0m \033[1;33mYou're feeling lucky today!\033[0m          \033[1;36m║\033[0m\n";
        }
        else
        {
            std::cout << "\033[1;36m║\033[0m \033[1;90mAn empty treasure chest\033[0m            \033[1;36m║\033[0m\n";
            std::cout << "\033[1;36m║\033[0m \033[1;90mSomeone beat you to it...\033[0m          \033[1;36m║\033[0m\n";
            std::cout << "\033[1;36m║\033[0m \033[1;90mBetter luck next time!\033[0m             \033[1;36m║\033[0m\n";
        }
        break;

    case ENTRANCE:
        std::cout << "\033[1;36m║\033[0m \033[1;32mWelcome to the dungeon!\033[0m                \033[1;36m║\033[0m\n";
        std::cout << "\033[1;36m║\033[0m \033[1;32mThe entrance sealed behind\033[0m             \033[1;36m║\033[0m\n";
        std::cout << "\033[1;36m║\033[0m \033[1;32myou. Find the exit to escape!\033[0m          \033[1;36m║\033[0m\n";
        break;

    case EXIT:
        std::cout << "\033[1;36m║\033[0m \033[1;36mThe exit stands before you!          \033[0m\033[1;36m║\033[0m\n";
        std::cout << "\033[1;36m║\033[0m \033[1;36mYou've conquered the dungeon\033[0m         \033[1;36m║\033[0m\n";
        std::cout << "\033[1;36m║\033[0m \033[1;36mVictory is within your grasp!\033[0m        \033[1;36m║\033[0m\n";
        break;

    case EMPTY:
    default:
        std::cout << "\033[1;36m║\033[0m Nothing of interest here.            \033[1;36m║\033[0m\n";
        std::cout << "\033[1;36m║\033[0m Just an empty chamber with           \033[1;36m║\033[0m\n";
        std::cout << "\033[1;36m║\033[0m stone walls and silence.             \033[1;36m║\033[0m\n";
        break;
    }

    std::cout << "\033[1;36m╚══════════════════════════════════════╝\033[0m\n";

    // Show available exits in text form for clarity
    std::cout << "\nAvailable exits: ";
    if (northOpen)
        std::cout << "North(W) ";
    if (southOpen)
        std::cout << "South(S) ";
    if (eastOpen)
        std::cout << "East(D) ";
    if (westOpen)
        std::cout << "West(A) ";
    std::cout << "\n";
}

void Room::monsterEncounter(Player &player)
{
    // Skip if no monster or monster is already defeated
    if (!monster || !monster->isAlive())
    {
        return;
    }

    std::cout << "You encounter a " << monster->getName() << "!" << std::endl;
    monster->displayStats();

    // Start combat
    Combat::battle(player, *monster);

    // If player won, give rewards
    if (!monster->isAlive())
    {
        int xpReward = monster->getXPReward();
        std::cout << "You defeated the " << monster->getName() << " and gained " << xpReward << " XP!" << std::endl;
        player.addXP(xpReward);
        player.addScore(xpReward);
    }
}

void Room::trapEffect(Player &player)
{
    // Make sure this is a trap room
    if (type != TRAP_ROOM)
        return;

    // Only tell the player they avoided the trap if this is a revisit
    if (visited)
    {
        std::cout << "\033[1;32mYou carefully avoid the " << trapTypeToString() << " trap you discovered earlier.\033[0m" << std::endl;
        return;
    }

    // Set up the trap type description and visuals
    std::string trapDesc;
    std::string trapAnimation;
    std::string trapSound;
    std::string avoidAction;

    TrapType currentTrapType = trapType;
    switch (currentTrapType)
    {
    case SPIKE_TRAP:
        trapDesc = "Deadly spikes suddenly emerge from the floor!";
        trapAnimation = "⇧⇧⇧⇧ 🔪 ⇧⇧⇧⇧";
        trapSound = "*SHHHNK!*";
        avoidAction = "JUMP";
        break;
    case POISON_TRAP:
        trapDesc = "Poisonous gas begins filling the room!";
        trapAnimation = "☁️ ☠️  ☁️  ☁️ ";
        trapSound = "*HISSSS!*";
        avoidAction = "HOLD BREATH";
        break;
    case FALLING_ROCKS:
        trapDesc = "The ceiling begins to collapse!";
        trapAnimation = "⇩⇩⇩ 🪨 ⇩⇩⇩";
        trapSound = "*RUMBLE!*";
        avoidAction = "DODGE";
        break;
    case FLAME_TRAP:
        trapDesc = "Fire erupts from vents in the walls!";
        trapAnimation = "🔥 🔥 🔥 🔥";
        trapSound = "*WHOOSH!*";
        avoidAction = "ROLL";
        break;
    case ICE_TRAP:
        trapDesc = "The floor suddenly freezes with magical ice!";
        trapAnimation = "❄️ ❄️ ❄️ ❄️";
        trapSound = "*CRACKLE!*";
        avoidAction = "BRACE";
        break;
    }

    // Make it exciting! Let's show some trap animations
    std::cout << "\n\033[1;31m" << trapSound << "\033[0m" << std::endl;
    std::cout << "\033[1;33mA trap has been triggered!\033[0m" << std::endl;
    std::cout << "\033[1;31m" << trapDesc << "\033[0m" << std::endl;
    std::cout << "\n\033[1;33m" << trapAnimation << "\033[0m" << std::endl;
    std::cout << "\nYou have a chance to avoid it with quick reflexes!" << std::endl;

    // Give players a moment to react - builds tension!
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    // Just use first letter of action for simplicity
    char keyToPress = avoidAction[0]; // J for JUMP, D for DODGE, etc.

    // Classic countdown - like in the movies!
    std::cout << "\nGet ready..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "3..." << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "2..." << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "1..." << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // Show action command with visual emphasis
    std::cout << "\n\n\033[1;31m" << avoidAction << "!\033[0m Press '" << keyToPress << "' quickly!" << std::endl;

    // Get player input with timeout
    char playerInput = '0';
    bool inputReceived = false;

    // Set terminal to non-canonical mode to get immediate input
    system("stty -icanon min 1 time 0");

    // Start time
    auto startTime = std::chrono::high_resolution_clock::now();

    // Give player 1.5 seconds to react (15 checks of 100ms each)
    for (int i = 0; i < 15; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (std::cin.peek() != EOF)
        {
            std::cin >> playerInput;
            inputReceived = true;
            playerInput = toupper(playerInput);
            break;
        }
    }

    // Reset terminal mode
    system("stty icanon");

    // Clear any remaining input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Calculate reaction time for scoring
    auto endTime = std::chrono::high_resolution_clock::now();
    auto reactionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Check if player avoided trap
    if (inputReceived && playerInput == toupper(keyToPress))
    {
        // Successfully avoided trap
        std::cout << "\n\033[1;32mYou successfully avoided the trap with incredible reflexes!\033[0m" << std::endl;
        std::cout << "Reaction time: \033[1;36m" << reactionTime << "ms\033[0m" << std::endl;

        // Calculate reward based on reaction time (faster = better reward)
        int baseXP = trapDamage;
        int timeBonus = std::max(0, 1000 - static_cast<int>(reactionTime)) / 10;
        int xpGain = baseXP + timeBonus;

        std::cout << "You gained \033[1;32m" << xpGain << "\033[0m XP and \033[1;33m" << xpGain / 2 << "\033[0m gold for your reflexes!" << std::endl;
        player.addXP(xpGain);
        player.addGold(xpGain / 2);  // Also give some gold as reward
        player.addScore(xpGain * 2); // Double score for avoiding a trap
    }
    else
    {
        // Failed to avoid trap
        std::cout << "\n\033[1;31mYou couldn't avoid the " << trapTypeToString() << " trap!\033[0m" << std::endl;

        // Different trap effects based on type
        TrapType currentTrapType = trapType;
        switch (currentTrapType)
        {
        case SPIKE_TRAP:
            std::cout << "\033[1;31mThe spikes pierce through your defenses!\033[0m" << std::endl;
            break;
        case POISON_TRAP:
            std::cout << "\033[1;31mThe poisonous gas makes you cough and choke!\033[0m" << std::endl;
            break;
        case FALLING_ROCKS:
            std::cout << "\033[1;31mThe falling rocks crash down on you!\033[0m" << std::endl;
            break;
        case FLAME_TRAP:
            std::cout << "\033[1;31mThe flames sear your skin!\033[0m" << std::endl;
            break;
        case ICE_TRAP:
            std::cout << "\033[1;31mThe freezing cold bites into your flesh!\033[0m" << std::endl;
            break;
        }

        std::cout << "You take \033[1;31m" << trapDamage << "\033[0m damage." << std::endl;
        player.takeDamage(trapDamage);

        // Consolation XP for experiencing a trap
        int xpGain = trapDamage / 2;
        std::cout << "You gained \033[1;32m" << xpGain << "\033[0m XP from the painful experience." << std::endl;
        player.addXP(xpGain);
        player.addScore(xpGain); // Add score for surviving a trap
    }
}

void Room::treasureFound(Player &player)
{
    // Skip if treasure has already been taken
    if (!treasure)
    {
        std::cout << "The treasure chest has already been looted." << std::endl;
        return;
    }

    std::cout << "\n\033[1;33mYou found a treasure chest containing:\033[0m " << std::endl;
    treasure->display();

    // Add item to inventory if there's space
    if (player.getInventory().addItem(treasure))
    {
        std::cout << "\033[1;32mItem added to your inventory!\033[0m" << std::endl;
        treasure = nullptr; // Set to null since ownership transferred to inventory

        // Add additional gold bonus
        int goldBonus = 20 + (rand() % 30); // Random gold bonus between 20-50
        std::cout << "You also found \033[1;33m" << goldBonus << " gold coins\033[0m in the chest!" << std::endl;
        player.addGold(goldBonus);
        player.addScore(50 + goldBonus); // Add score for finding treasure
    }
    else
    {
        std::cout << "\033[1;31mYour inventory is full. You cannot pick up the item.\033[0m" << std::endl;
    }
}

void Room::applyRoomEffect(Player &player)
{
    // Apply different effects based on room type
    switch (type)
    {
    case HEALING_ROOM:
    {
        int healAmount = player.getMaxHP() / 4; // Heal 25% of max HP
        std::cout << "\n\033[1;32mYou drink from the healing fountain...\033[0m" << std::endl;
        std::cout << "\033[1;32mThe mystical waters restore " << healAmount << " HP!\033[0m" << std::endl;
        player.heal(healAmount);
        std::cout << "Your current HP: " << player.getHP() << "/" << player.getMaxHP() << std::endl;
    }
    break;

    case CHALLENGE_ROOM:
    {
        std::cout << "\n\033[1;35mWelcome to the CHALLENGE ROOM!\033[0m" << std::endl;
        std::cout << "Complete a challenge to earn bonus rewards." << std::endl;

        // Random challenge
        int challengeType = rand() % 3;
        switch (challengeType)
        {
        case 0: // Memory challenge
        {
            std::cout << "\n\033[1;35mMEMORY CHALLENGE:\033[0m Remember the sequence!" << std::endl;

            // Generate random sequence
            std::string sequence = "";
            for (int i = 0; i < 5; i++)
            {
                char randomChar = 'A' + (rand() % 5); // A to E
                sequence += randomChar;
            }

            std::cout << "Memorize: \033[1;33m" << sequence << "\033[0m" << std::endl;
            std::cout << "You have 5 seconds..." << std::endl;

            // Show for 5 seconds then clear
            std::this_thread::sleep_for(std::chrono::seconds(5));
            Utils::clearScreen();

            std::cout << "\033[1;35mMEMORY CHALLENGE:\033[0m What was the sequence?" << std::endl;
            std::cout << "Enter your answer: ";

            std::string playerGuess;
            std::getline(std::cin, playerGuess);

            // Convert to uppercase for comparison
            std::transform(playerGuess.begin(), playerGuess.end(), playerGuess.begin(), ::toupper);

            if (playerGuess == sequence)
            {
                std::cout << "\033[1;32mCorrect! You have an excellent memory!\033[0m" << std::endl;
                int reward = 50 + (player.getLevel() * 10);
                std::cout << "You earned \033[1;32m" << reward << " XP\033[0m and \033[1;33m" << reward << " gold!\033[0m" << std::endl;
                player.addXP(reward);
                player.addGold(reward);
                player.addScore(reward * 2);
            }
            else
            {
                std::cout << "\033[1;31mIncorrect! The sequence was \033[1;33m" << sequence << "\033[0m" << std::endl;
                std::cout << "Better luck next time!" << std::endl;
            }
        }
        break;

        case 1: // Riddle challenge
        {
            std::cout << "\n\033[1;35mRIDDLE CHALLENGE:\033[0m Answer correctly to earn a reward!" << std::endl;

            // Array of riddles and answers
            struct Riddle
            {
                std::string question;
                std::string answer;
            };

            Riddle riddles[] = {
                {"I'm tall when I'm young, and short when I'm old. What am I?", "candle"},
                {"What has keys but no locks, space but no room, and you can enter but not go in?", "keyboard"},
                {"What gets wetter as it dries?", "towel"},
                {"What has a head, a tail, is brown, and has no legs?", "penny"},
                {"What has many keys but can't open a single lock?", "piano"}};

            // Choose a random riddle
            int riddleIndex = rand() % 5;
            Riddle currentRiddle = riddles[riddleIndex];

            std::cout << "\033[1;33m" << currentRiddle.question << "\033[0m" << std::endl;
            std::cout << "Your answer: ";

            std::string playerAnswer;
            std::getline(std::cin, playerAnswer);

            // Convert to lowercase for comparison
            std::transform(playerAnswer.begin(), playerAnswer.end(), playerAnswer.begin(), ::tolower);

            if (playerAnswer == currentRiddle.answer)
            {
                std::cout << "\033[1;32mCorrect! You solved the riddle!\033[0m" << std::endl;
                int reward = 75 + (player.getLevel() * 15);
                std::cout << "You earned \033[1;32m" << reward << " XP\033[0m and \033[1;33m" << reward << " gold!\033[0m" << std::endl;
                player.addXP(reward);
                player.addGold(reward);
                player.addScore(reward * 2);
            }
            else
            {
                std::cout << "\033[1;31mIncorrect! The answer was '" << currentRiddle.answer << "'\033[0m" << std::endl;
                std::cout << "Better luck next time!" << std::endl;
            }
        }
        break;

        case 2: // Number guessing challenge
        {
            std::cout << "\n\033[1;35mNUMBER CHALLENGE:\033[0m Guess the number between 1 and 10" << std::endl;
            std::cout << "You have 3 attempts!" << std::endl;

            int secretNumber = 1 + (rand() % 10); // Random number 1-10
            bool guessedCorrectly = false;

            for (int attempt = 1; attempt <= 3; attempt++)
            {
                std::cout << "Attempt " << attempt << "/3. Enter your guess: ";

                int playerGuess;
                std::cin >> playerGuess;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (playerGuess == secretNumber)
                {
                    std::cout << "\033[1;32mCorrect! The number was " << secretNumber << "!\033[0m" << std::endl;
                    int reward = 100 - ((attempt - 1) * 25); // 100, 75, or 50 XP based on attempts used
                    std::cout << "You earned \033[1;32m" << reward << " XP\033[0m and \033[1;33m" << reward << " gold!\033[0m" << std::endl;
                    player.addXP(reward);
                    player.addGold(reward);
                    player.addScore(reward * 2);
                    guessedCorrectly = true;
                    break;
                }
                else if (playerGuess < secretNumber)
                {
                    std::cout << "\033[1;33mToo low!\033[0m" << std::endl;
                }
                else
                {
                    std::cout << "\033[1;33mToo high!\033[0m" << std::endl;
                }
            }

            if (!guessedCorrectly)
            {
                std::cout << "\033[1;31mOut of attempts! The number was " << secretNumber << ".\033[0m" << std::endl;
                std::cout << "Better luck next time!" << std::endl;
            }
        }
        break;
        }
    }
    break;

    case MERCHANT_ROOM:
    {
        std::cout << "\n\033[1;33mWelcome to the MERCHANT!\033[0m" << std::endl;
        std::cout << "The merchant offers various items and services." << std::endl;
        std::cout << "Would you like to browse their wares? (Y/N): ";

        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (toupper(choice) == 'Y')
        {
            // Show merchant inventory
            std::cout << "\n\033[1;33mMERCHANT INVENTORY:\033[0m" << std::endl;
            std::cout << "1. Health Potion (Restores 50 HP) - 20 gold" << std::endl;
            std::cout << "2. Strength Elixir (+5 Attack) - 30 gold" << std::endl;
            std::cout << "3. Defense Charm (+5 Defense) - 30 gold" << std::endl;
            std::cout << "4. Magic Map (Reveals nearby rooms) - 50 gold" << std::endl;
            std::cout << "\nYour gold: \033[1;33m" << player.getGold() << "\033[0m" << std::endl;
            std::cout << "Enter choice (1-4) or 0 to exit: ";

            int itemChoice;
            std::cin >> itemChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (itemChoice)
            {
            case 1: // Health Potion
                if (player.getGold() >= 20)
                {
                    player.addGold(-20); // Deduct gold
                    // Create health potion and add to inventory
                    Item *potion = new Item("Health Potion", HEALTH_POTION, 50, "Restores 50 HP when used");
                    if (player.getInventory().addItem(potion))
                    {
                        std::cout << "\033[1;32mYou purchased a Health Potion!\033[0m" << std::endl;
                    }
                    else
                    {
                        std::cout << "Your inventory is full!" << std::endl;
                        delete potion;      // Clean up if inventory is full
                        player.addGold(20); // Refund gold
                    }
                }
                else
                {
                    std::cout << "\033[1;31mNot enough gold!\033[0m" << std::endl;
                }
                break;

            case 2: // Strength Elixir
                if (player.getGold() >= 30)
                {
                    player.addGold(-30);
                    player.setAttack(player.getAttack() + 5);
                    std::cout << "\033[1;32mYou purchased a Strength Elixir!\033[0m" << std::endl;
                    std::cout << "Your attack increased by 5!" << std::endl;
                }
                else
                {
                    std::cout << "\033[1;31mNot enough gold!\033[0m" << std::endl;
                }
                break;

            case 3: // Defense Charm
                if (player.getGold() >= 30)
                {
                    player.addGold(-30);
                    player.setDefense(player.getDefense() + 5);
                    std::cout << "\033[1;32mYou purchased a Defense Charm!\033[0m" << std::endl;
                    std::cout << "Your defense increased by 5!" << std::endl;
                }
                else
                {
                    std::cout << "\033[1;31mNot enough gold!\033[0m" << std::endl;
                }
                break;

            case 4: // Magic Map
                if (player.getGold() >= 50)
                {
                    player.addGold(-50);
                    // Logic to reveal nearby rooms would go here
                    std::cout << "\033[1;32mYou purchased a Magic Map!\033[0m" << std::endl;
                    std::cout << "The map reveals nearby rooms!" << std::endl;
                    // This would need to be implemented in the dungeon class
                }
                else
                {
                    std::cout << "\033[1;31mNot enough gold!\033[0m" << std::endl;
                }
                break;

            case 0:
                std::cout << "Thank you for browsing!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice." << std::endl;
            }
        }
        else
        {
            std::cout << "Come back if you change your mind!" << std::endl;
        }
    }
    break;

    case ENTRANCE:
        // no announcement banner for entrance rooms
        break;

    case EMPTY:
        // no announcement banner for empty rooms
        break;
    }
}

char Room::getMapChar() const
{
    if (!visited)
    {
        return '?'; // Unknown room
    }

    switch (type)
    {
    case MONSTER_ROOM:
        return (monster && monster->isAlive()) ? 'M' : 'm';
    case TRAP_ROOM:
        return 'T';
    case TREASURE_ROOM:
        return (treasure) ? '$' : 'o';
    case ENTRANCE:
        return 'E';
    case EXIT:
        return 'X';
    case HEALING_ROOM:
        return 'H';
    case CHALLENGE_ROOM:
        return 'C';
    case MERCHANT_ROOM:
        return 'S'; // S for shop
    case EMPTY:
    default:
        return '.';
    }
}
