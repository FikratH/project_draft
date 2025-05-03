# Dungeon Crawler Adventure

A text-based roguelike game where you explore procedurally-generated dungeons, fight monsters, disarm traps, collect loot, and try to reach the exit alive!

## Team Members

- Adalat Babayev (3036364567)
- Amil Milevskiy (3036407955)
- Asset Mukayev (3036384608)
- Fikrat Huseynov (3036384842)
- Surkhay Malikov (3036385573)
- Vladislav Prokopev (3036440438)

## Game Description

Dungeon Crawler Adventure is an immersive text-based roguelike game that combines strategic decision-making with randomized dungeon exploration. The player navigates through a procedurally generated maze-like dungeon filled with monsters, traps, treasures, and special rooms. Each playthrough offers a unique experience with different dungeon layouts, monster encounters, and treasure finds.

The game features an intuitive user interface with colorful ASCII art and a visual dungeon map that updates as the player explores. Combat is turn-based, allowing for tactical decisions, and the player can collect and use various items to aid in their quest. The ultimate goal is to find the exit while gathering as much treasure and experience as possible.

## Game Features

- **Procedurally generated dungeons** using a depth-first search maze algorithm
- **Three difficulty levels**: Easy, Medium, and Hard (affects map size, monster strength, etc.)
- **Turn-based combat system** with various monsters to fight
- **Multiple room types**:
  - Monster rooms with creatures to battle
  - Trap rooms with reflex-based challenges
  - Treasure rooms with valuable loot
  - Healing fountains to restore health
  - Challenge rooms with special encounters
  - Merchant shops to buy and sell items
- **Experience and leveling system** that increases your stats as you progress
- **Inventory system** to manage and use items you collect
- **Save/Load functionality** to continue your adventure later
- **Leaderboard system** to track high scores

## Module Structure

The game is structured into multiple modules, each handling a specific aspect of gameplay:

- `utils.h/cpp`: Utility functions for input validation, RNG, etc.
- `difficulty.h/cpp`: Manages different difficulty levels and their parameters
- `entity.h/cpp`: Base class for any creature with HP, attack, and defense
- `player.h/cpp`: Player character with XP, leveling, inventory management
- `item.h/cpp`: Defines consumable items and their effects
- `inventory.h/cpp`: Manages the player's collection of items
- `monster.h/cpp`: Defines enemy types and a factory to create them
- `room.h/cpp`: Represents dungeon rooms with different events
- `dungeon.h/cpp`: Generates and manages the 2D maze of rooms
- `combat.h/cpp`: Handles turn-based combat between entities
- `io.h/cpp`: Provides file I/O for saving/loading and leaderboard

## Libraries Used

The game uses only standard C++ libraries:
- `iostream`, `iomanip` - For input/output operations and formatting
- `fstream`, `sstream` - For file handling and string stream operations
- `vector`, `string` - For data structures
- `algorithm` - For various algorithms (sort, find, etc.)
- `ctime`, `chrono` - For time-related functions
- `cmath` - For mathematical operations
- `thread` - For thread management (used in animations and timing)
- `limits` - For numeric limits
- `utility` - For pair and move semantics

## How to Compile and Run

The project includes a Makefile for easy compilation. To compile and run the game:

```bash
# Compile the game
make

# Run the game
./dungeon_crawler
# OR
make run
```

To clean up compiled files:

```bash
make clean
```

## Controls

- `W`, `A`, `S`, `D`: Move north, west, south, east
- `I`: View inventory
- `P`: View player stats
- `V`: Save game
- `Q`: Quit to main menu

During combat and special encounters, follow the on-screen prompts for available actions.

## Save/Load System

The game automatically creates save files named after your character. You can load these saves from the main menu. Your progress, including dungeon state, inventory, and player stats, will be preserved.
