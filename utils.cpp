#include "utils.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <thread>

// Initialize the random number generator with a seed
std::mt19937 Utils::rng(std::chrono::steady_clock::now().time_since_epoch().count());

int Utils::getValidInput(int min, int max)
{
    int input;
    while (true)
    {
        if (std::cin >> input)
        {
            // Clear the input buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Check if input is within range
            if (input >= min && input <= max)
            {
                return input;
            }
            else
            {
                std::cout << "Input must be between " << min << " and " << max << ". Try again: ";
            }
        }
        else
        {
            // Clear the error flag
            std::cin.clear();
            // Discard invalid input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        }
    }
}

int Utils::getRandomInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

float Utils::getRandomFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

bool Utils::checkProbability(float probability)
{
    return getRandomFloat(0.0f, 1.0f) < probability;
}

void Utils::clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif

    // Add a small pause to ensure the screen is cleared properly
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void Utils::log(const std::string &message, bool toFile)
{
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << "[" << std::ctime(&time) << "] " << message;

    // Log to console
    std::cout << ss.str() << std::endl;

    // Log to file if requested
    if (toFile)
    {
        std::ofstream logFile("game_log.txt", std::ios::app);
        if (logFile.is_open())
        {
            logFile << ss.str() << std::endl;
            logFile.close();
        }
    }
}

void Utils::slowPrint(const std::string& message, int delayMs)
{
    for (char c : message)
    {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void Utils::slowVerticalPrint(const std::string& multilineText, int delayMs) {
    std::istringstream stream(multilineText);
    std::string line;
    std::vector<std::string> lines;

    // Split into lines
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    // Print one line at a time with delay
    for (const auto& l : lines) {
        std::cout << l << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}


std::vector<std::string> Utils::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

std::string Utils::join(const std::vector<std::string> &v, char delimiter)
{
    std::string result;
    for (size_t i = 0; i < v.size(); ++i)
    {
        result += v[i];
        if (i < v.size() - 1)
        {
            result += delimiter;
        }
    }
    return result;
}
