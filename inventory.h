#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "item.h"

class Inventory {
public:
    // Constructor
    Inventory(int maxSize = 10);
    
    // Destructor
    ~Inventory();
    
    // Add an item to the inventory
    bool addItem(Item* item);
    
    // Remove an item from the inventory at the specified index
    Item* removeItem(int index);
    
    // Use an item at the specified index
    bool useItem(int index, Player& player);
    
    // Display all items in the inventory
    void display() const;
    
    // Check if the inventory is empty
    bool isEmpty() const;
    
    // Check if the inventory is full
    bool isFull() const;
    
    // Get the number of items in the inventory
    int getItemCount() const;
    
    // Get the maximum size of the inventory
    int getMaxSize() const;
    
    // Get an item at the specified index
    Item* getItemAt(int index) const;
    
private:
    std::vector<Item*> items;
    int maxSize;
};

#endif // INVENTORY_H
