#include "dungeon.h"
#include "utils.h"
#include <iostream>
#include <stack>
#include <algorithm>
#include <ctime>

Dungeon::Dungeon(Difficulty difficulty)
    : difficulty(difficulty), diffConfig(difficulty) {
    mapSize = diffConfig.getMapSize();
    currentX = 0;
    currentY = 0;
    
    // Initialize the dungeon
    initialize();
}

Dungeon::~Dungeon() {
    // Clean up dynamically allocated rooms
    for (auto& row : rooms) {
        for (auto& room : row) {
            delete room;
        }
    }
}

void Dungeon::initialize() {
    // Resize the grid to mapSize x mapSize
    rooms.resize(mapSize, std::vector<Room*>(mapSize, nullptr));
    
    // Create rooms
    for (int y = 0; y < mapSize; ++y) {
        for (int x = 0; x < mapSize; ++x) {
            rooms[y][x] = new Room(x, y);
        }
    }
    
    // Generate the maze structure
    generateMaze();
    
    // Populate rooms with content
    populateRooms();
    
    // Set entrance and exit
    rooms[0][0]->setType(ENTRANCE);
    rooms[mapSize-1][mapSize-1]->setType(EXIT);
    
    // Start at the entrance
    currentX = 0;
    currentY = 0;
    rooms[currentY][currentX]->setVisited(true);
}

bool Dungeon::isValidPosition(int x, int y) const {
    return (x >= 0 && x < mapSize && y >= 0 && y < mapSize);
}

std::vector<std::pair<int, int>> Dungeon::getUnconnectedAdjacent(int x, int y) const {
    std::vector<std::pair<int, int>> adjacent;
    
    // Check North
    if (isValidPosition(x, y-1) && !rooms[y-1][x]->isVisited()) {
        adjacent.push_back({x, y-1});
    }
    
    // Check South
    if (isValidPosition(x, y+1) && !rooms[y+1][x]->isVisited()) {
        adjacent.push_back({x, y+1});
    }
    
    // Check East
    if (isValidPosition(x+1, y) && !rooms[y][x+1]->isVisited()) {
        adjacent.push_back({x+1, y});
    }
    
    // Check West
    if (isValidPosition(x-1, y) && !rooms[y][x-1]->isVisited()) {
        adjacent.push_back({x-1, y});
    }
    
    return adjacent;
}

void Dungeon::connectRooms(int x1, int y1, int x2, int y2) {
    // Determine the direction
    if (x1 == x2) {
        if (y1 < y2) {
            // Connect South
            rooms[y1][x1]->setSouthOpen(true);
            rooms[y2][x2]->setNorthOpen(true);
        } else {
            // Connect North
            rooms[y1][x1]->setNorthOpen(true);
            rooms[y2][x2]->setSouthOpen(true);
        }
    } else {
        if (x1 < x2) {
            // Connect East
            rooms[y1][x1]->setEastOpen(true);
            rooms[y2][x2]->setWestOpen(true);
        } else {
            // Connect West
            rooms[y1][x1]->setWestOpen(true);
            rooms[y2][x2]->setEastOpen(true);
        }
    }
}

void Dungeon::generateMaze() {
    // Depth-first search maze generation algorithm
    
    // Mark all rooms as unvisited
    for (int y = 0; y < mapSize; ++y) {
        for (int x = 0; x < mapSize; ++x) {
            rooms[y][x]->setVisited(false);
        }
    }
    
    // Start from a random position
    int startX = Utils::getRandomInt(0, mapSize - 1);
    int startY = Utils::getRandomInt(0, mapSize - 1);
    
    // For a more structured dungeon, always start from 0,0
    startX = 0;
    startY = 0;
    
    std::stack<std::pair<int, int>> stack;
    stack.push({startX, startY});
    rooms[startY][startX]->setVisited(true);
    
    // Continue until all cells are visited
    while (!stack.empty()) {
        int x = stack.top().first;
        int y = stack.top().second;
        
        // Get unvisited neighboring cells
        std::vector<std::pair<int, int>> neighbors = getUnconnectedAdjacent(x, y);
        
        if (!neighbors.empty()) {
            // Choose a random neighbor
            int next = Utils::getRandomInt(0, neighbors.size() - 1);
            int nextX = neighbors[next].first;
            int nextY = neighbors[next].second;
            
            // Connect the two cells
            connectRooms(x, y, nextX, nextY);
            
            // Mark the neighbor as visited and push it onto the stack
            rooms[nextY][nextX]->setVisited(true);
            stack.push({nextX, nextY});
        } else {
            // Backtrack
            stack.pop();
        }
    }
    
    // Reset visited status for all rooms
    for (int y = 0; y < mapSize; ++y) {
        for (int x = 0; x < mapSize; ++x) {
            rooms[y][x]->setVisited(false);
        }
    }
}

void Dungeon::populateRooms() {
    // Get difficulty parameters
    float monsterRate = diffConfig.getMonsterSpawnRate();
    float trapRate = diffConfig.getTrapSpawnRate();
    float lootRate = diffConfig.getLootSpawnRate();
    float monsterMult = diffConfig.getMonsterStrengthMultiplier();
    float trapMult = diffConfig.getTrapDamageMultiplier();
    float lootMult = diffConfig.getLootQualityMultiplier();
    
    // New special room rates (these are intentionally rare to make them special)
    float healingRate = 0.12f; // 12% chance
    float challengeRate = 0.15f; // 15% chance
    float merchantRate = 0.08f; // 8% chance
    
    // Counters to limit special room types
    int healingRooms = 0;
    int challengeRooms = 0;
    int merchantRooms = 0;
    
    // Maximum allowed of each type based on map size
    int maxHealingRooms = std::max(1, mapSize / 5); // 1 healing room per 5x5 area
    int maxChallengeRooms = std::max(2, mapSize / 4); // More challenge rooms
    int maxMerchantRooms = std::max(1, mapSize / 6); // Merchants are rare
    
    // First pass - place standard content
    for (int y = 0; y < mapSize; ++y) {
        for (int x = 0; x < mapSize; ++x) {
            // Skip entrance and exit
            if ((x == 0 && y == 0) || (x == mapSize-1 && y == mapSize-1)) {
                continue;
            }
            
            // Determine room type based on probabilities
            if (Utils::checkProbability(monsterRate)) {
                rooms[y][x]->setType(MONSTER_ROOM);
                Monster* monster = MonsterFactory::createRandomMonster(monsterMult);
                rooms[y][x]->setMonster(monster);
            } else if (Utils::checkProbability(trapRate)) {
                rooms[y][x]->setType(TRAP_ROOM);
                int baseDamage = 10 + (5 * (x + y) / 2); // Higher damage deeper in dungeon
                int damage = static_cast<int>(baseDamage * trapMult);
                rooms[y][x]->setTrapDamage(damage);
                
                // Set random trap type for variety
                TrapType trapType = static_cast<TrapType>(rand() % 5); // 5 trap types
                rooms[y][x]->setTrapType(trapType);
            } else if (Utils::checkProbability(lootRate)) {
                rooms[y][x]->setType(TREASURE_ROOM);
                Item* treasure = Item::createRandomItem(lootMult);
                rooms[y][x]->setTreasure(treasure);
            } else {
                rooms[y][x]->setType(EMPTY);
            }
        }
    }
    
    // Second pass - add special rooms by replacing some existing rooms
    // This ensures they're distributed throughout the dungeon
    for (int attempts = 0; attempts < 100 && (healingRooms < maxHealingRooms || 
                                           challengeRooms < maxChallengeRooms || 
                                           merchantRooms < maxMerchantRooms); attempts++) {
        // Pick a random room that's not entrance or exit
        int x = 1 + rand() % (mapSize - 2); // Avoid 0 and mapSize-1
        int y = 1 + rand() % (mapSize - 2); // Avoid 0 and mapSize-1
        
        // Skip if it's already a special room
        if (rooms[y][x]->getType() == HEALING_ROOM || 
            rooms[y][x]->getType() == CHALLENGE_ROOM || 
            rooms[y][x]->getType() == MERCHANT_ROOM) {
            continue;
        }
        
        // Try to place healing room
        if (healingRooms < maxHealingRooms && Utils::checkProbability(healingRate)) {
            // Replace current room with healing room
            Room* oldRoom = rooms[y][x];
            rooms[y][x] = new Room(x, y);
            rooms[y][x]->setType(HEALING_ROOM);
            rooms[y][x]->setNorthOpen(oldRoom->isNorthOpen());
            rooms[y][x]->setSouthOpen(oldRoom->isSouthOpen());
            rooms[y][x]->setEastOpen(oldRoom->isEastOpen());
            rooms[y][x]->setWestOpen(oldRoom->isWestOpen());
            delete oldRoom;
            healingRooms++;
        }
        // Try to place challenge room
        else if (challengeRooms < maxChallengeRooms && Utils::checkProbability(challengeRate)) {
            // Replace current room with challenge room
            Room* oldRoom = rooms[y][x];
            rooms[y][x] = new Room(x, y);
            rooms[y][x]->setType(CHALLENGE_ROOM);
            rooms[y][x]->setNorthOpen(oldRoom->isNorthOpen());
            rooms[y][x]->setSouthOpen(oldRoom->isSouthOpen());
            rooms[y][x]->setEastOpen(oldRoom->isEastOpen());
            rooms[y][x]->setWestOpen(oldRoom->isWestOpen());
            delete oldRoom;
            challengeRooms++;
        }
        // Try to place merchant room
        else if (merchantRooms < maxMerchantRooms && Utils::checkProbability(merchantRate)) {
            // Replace current room with merchant room
            Room* oldRoom = rooms[y][x];
            rooms[y][x] = new Room(x, y);
            rooms[y][x]->setType(MERCHANT_ROOM);
            rooms[y][x]->setNorthOpen(oldRoom->isNorthOpen());
            rooms[y][x]->setSouthOpen(oldRoom->isSouthOpen());
            rooms[y][x]->setEastOpen(oldRoom->isEastOpen());
            rooms[y][x]->setWestOpen(oldRoom->isWestOpen());
            delete oldRoom;
            merchantRooms++;
        }
    }
    
    std::cout << "Dungeon populated with:" << std::endl;
    std::cout << "- " << healingRooms << " healing fountains" << std::endl; 
    std::cout << "- " << challengeRooms << " challenge rooms" << std::endl;
    std::cout << "- " << merchantRooms << " merchant shops" << std::endl;
    std::cout << "Find them all to maximize your adventure!" << std::endl;
}

bool Dungeon::movePlayer(char direction, Player& player, bool showRoom) {
    int newX = currentX;
    int newY = currentY;
    
    // Calculate new position based on direction
    switch (toupper(direction)) {
        case 'W': // North
            if (!rooms[currentY][currentX]->isNorthOpen()) {
                return false;
            }
            newY--;
            break;
            
        case 'S': // South
            if (!rooms[currentY][currentX]->isSouthOpen()) {
                return false;
            }
            newY++;
            break;
            
        case 'D': // East
            if (!rooms[currentY][currentX]->isEastOpen()) {
                return false;
            }
            newX++;
            break;
            
        case 'A': // West
            if (!rooms[currentY][currentX]->isWestOpen()) {
                return false;
            }
            newX--;
            break;
            
        default:
            return false;
    }
    
    // Check if new position is valid
    if (!isValidPosition(newX, newY)) {
        return false;
    }
    
    // Move to the new room
    currentX = newX;
    currentY = newY;
    
    // Only enter the room and trigger events if it's a special room or showRoom is true
    Room* newRoom = rooms[currentY][currentX];
    
    // Check if this is a room with a special event (monster, trap, treasure, exit)
    RoomType type = newRoom->getType();
    bool isSpecialRoom = (type == MONSTER_ROOM && newRoom->getMonster() && newRoom->getMonster()->isAlive()) ||
                         (type == TRAP_ROOM) || // Always trigger trap rooms
                         (type == TREASURE_ROOM && newRoom->getTreasure()) ||
                         (type == EXIT) ||
                         (type == HEALING_ROOM) ||
                         (type == CHALLENGE_ROOM) ||
                         (type == MERCHANT_ROOM);
    
    if (showRoom || isSpecialRoom) {
        // Show detailed room view and trigger events for special rooms or when explicitly requested
        Utils::clearScreen();
        newRoom->enter(player);
    } else {
        // Just mark the room as visited without showing detailed view
        newRoom->setVisited(true);
    }
    
    return true;
}

void Dungeon::displayCurrentRoom() const {
    rooms[currentY][currentX]->display();
}

void Dungeon::displayMap() const {
    Utils::clearScreen();
    std::cout << "\n╔═════ DUNGEON MAP ══════╗\n";
    std::cout << "║ Legend:                ║\n";
    std::cout << "║  🏠 (E) - Entrance     ║\n";
    std::cout << "║  🚪 (X) - Exit         ║\n";
    std::cout << "║  👤 (@) - You          ║\n";
    std::cout << "║  👹 (M) - Monster      ║\n";
    std::cout << "║  💀 (m) - Dead Monster ║\n";
    std::cout << "║  ⚠️  (T) - Trap         ║\n";
    std::cout << "║  💰 ($) - Treasure     ║\n";
    std::cout << "║  📦 (o) - Empty Chest  ║\n";
    std::cout << "║  ⬜ (.) - Empty Room   ║\n";
    std::cout << "║  ❓ (?) - Unexplored   ║\n";
    std::cout << "╚════════════════════════╝\n\n";
    
    // Print the column numbers on top
    std::cout << "  "; // Space for row number
    for (int x = 0; x < mapSize; ++x) {
        std::cout << "  " << x << " ";
    }
    std::cout << "\n";
    
    for (int y = 0; y < mapSize; ++y) {
        // Start with top walls (no row number here)
        std::cout << "  ";
        
        // Top walls
        for (int x = 0; x < mapSize; ++x) {
            std::cout << "+";
            std::cout << (rooms[y][x]->isNorthOpen() ? "   " : "---");
        }
        std::cout << "+\n";
        
        // Print row number before content
        std::cout << y << " ";
        
        // Room contents and side walls
        for (int x = 0; x < mapSize; ++x) {
            std::cout << (rooms[y][x]->isWestOpen() ? " " : "|");
            
            // Current position
            if (x == currentX && y == currentY) {
                std::cout << "Ω";
            } else {
                char mapChar = rooms[y][x]->getMapChar();
                // Add some visual enhancement around the characters
                switch(mapChar) {
                    case 'E': std::cout << " E "; break; // Entrance
                    case 'X': std::cout << " X "; break; // Exit
                    case 'M': std::cout << " M "; break; // Monster
                    case 'm': std::cout << " m "; break; // Dead monster
                    case 'T': std::cout << " T "; break; // Trap
                    case '$': std::cout << " $ "; break; // Treasure
                    case 'o': std::cout << " o "; break; // Empty chest
                    case '.': std::cout << "   "; break; // Empty room
                    case '?': std::cout << " ? "; break; // Unexplored
                    default: std::cout << " " << mapChar << " ";
                }
            }
        }
        std::cout << "|\n";
    }
    
    // Bottom walls for the last row
    std::cout << "   ";
    for (int x = 0; x < mapSize; ++x) {
        std::cout << "+";
        std::cout << (rooms[mapSize-1][x]->isSouthOpen() ? "   " : "---");
    }
    std::cout << "+\n";
    
    // Print the column numbers on bottom
    std::cout << "  ";
    for (int x = 0; x < mapSize; ++x) {
        std::cout << "  " << x << " ";
    }
    std::cout << "\n\n";
    
    // Player stats summary at the bottom of the map
    std::cout << "╔═════ PLAYER STATUS ═════╗\n";
    std::cout << "║ Position: (" << currentX << "," << currentY << ")         ║\n";
    std::cout << "╚═════════════════════════╝\n";
}

bool Dungeon::hasReachedExit() const {
    return (currentX == mapSize-1 && currentY == mapSize-1);
}

int Dungeon::getMapSize() const {
    return mapSize;
}

int Dungeon::getCurrentX() const {
    return currentX;
}

int Dungeon::getCurrentY() const {
    return currentY;
}

Difficulty Dungeon::getDifficulty() const {
    return difficulty;
}

Room* Dungeon::getRoomAt(int x, int y) const {
    if (isValidPosition(x, y)) {
        return rooms[y][x];
    }
    return nullptr;
}

void Dungeon::setCurrentPosition(int x, int y) {
    if (isValidPosition(x, y)) {
        currentX = x;
        currentY = y;
    }
}
