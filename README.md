# Dungeon Crawler Adventure

A text-based roguelike game where you explore procedurally-generated dungeons, fight monsters, disarm traps, collect loot, and try to reach the exit alive!

## Game Features

- **Procedurally generated dungeons** using a depth-first search maze algorithm
- **Three difficulty levels**: Easy, Medium, and Hard (affects map size, monster strength, etc.)
- **Turn-based combat system** with various monsters to fight
- **Trap and treasure rooms** that add variety to your adventure
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

## How to Compile

The project includes a Makefile for easy compilation. Simply run:

```bash
make
```

This will compile all the source files and create an executable named `dungeon_crawler`.

## How to Play

Run the executable:

```bash
./dungeon_crawler
```

### Controls:
- Select options from menus using numbers.
- Move through the dungeon using W (north), A (west), S (south), D (east).
- Battle monsters, find treasures, and try to reach the exit!
- Use items from your inventory to heal or boost your stats.

### Game Symbols:
- `@`: Your current position
- `E`: Dungeon entrance
- `X`: Dungeon exit
- `M`: Monster (alive)
- `m`: Defeated monster
- `T`: Trap
- `$`: Treasure (not looted)
- `o`: Empty treasure chest
- `.`: Empty room
- `?`: Unexplored room

## Project Requirements Implemented

- Random Events: Room triggers (combat, traps, loot)
- Data Structures: 2D grid of rooms, vectors for inventory, maps for leaderboard
- Dynamic Memory: Rooms, monsters, and items allocated/deallocated at runtime
- File I/O: Saving/loading game state and leaderboard data
- Multi-File Design: Each feature in its own header+source file
- Multiple Difficulty Levels: Customizable game experience

## Tips for Playing

- Start with Easy difficulty to learn the game mechanics.
- Collect items to help you survive tougher battles.
- Remember that you can save your game and continue later.
- Try to reach higher levels before tackling the exit on harder difficulties.
- Keep an eye on your HP during combat - know when to run away!

Good luck, adventurer!
