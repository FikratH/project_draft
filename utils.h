#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <random>

class Utils {
public:
    // Get valid integer input within a range
    static int getValidInput(int min, int max);
    
    // Generate a random integer between min and max (inclusive)
    static int getRandomInt(int min, int max);
    
    // Generate a random float between min and max
    static float getRandomFloat(float min, float max);
    
    // Return true with the given probability (0.0 to 1.0)
    static bool checkProbability(float probability);
    
    // Clear the console screen (cross-platform)
    static void clearScreen();
    
    // Log a message with timestamp to console (and optionally to file)
    static void log(const std::string& message, bool toFile = false);
    
    // Split a string by delimiter
    static std::vector<std::string> split(const std::string& s, char delimiter);
    
    // Join a vector of strings with a delimiter
    static std::string join(const std::vector<std::string>& v, char delimiter);

private:
    // Random number generator
    static std::mt19937 rng;
};

#endif // UTILS_H
