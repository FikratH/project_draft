# Dungeon Crawler Build System
# Let's keep this build organized by grouping related files

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = dungeon_crawler

# Group files by their logical categories

# Core system files
CORE_SRCS = utils.cpp difficulty.cpp io.cpp

# Entity-related files
ENTITY_SRCS = entity.cpp player.cpp monster.cpp item.cpp inventory.cpp combat.cpp

# World and environment files
WORLD_SRCS = room.cpp dungeon.cpp 

# Main file and UI
MAIN_SRCS = main.cpp

# Combine all source files
SRCS = $(MAIN_SRCS) $(CORE_SRCS) $(ENTITY_SRCS) $(WORLD_SRCS)
OBJS = $(SRCS:.cpp=.o)

# Build targets
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking the dungeon crawler game..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete! Run ./$(TARGET) to start your adventure!"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up the dungeon..."
	rm -f $(OBJS) $(TARGET)
	@echo "All clean!"

# Run the game directly
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
