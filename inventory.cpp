#include "inventory.h"
#include "player.h"
#include <iostream>

Inventory::Inventory(int maxSize) : maxSize(maxSize) {
    items.reserve(maxSize);
}

Inventory::~Inventory() {
    // Clean up all item pointers
    for (Item* item : items) {
        delete item;
    }
    items.clear();
}

bool Inventory::addItem(Item* item) {
    if (isFull()) {
        std::cout << "Inventory is full! Cannot add " << item->getName() << "." << std::endl;
        return false;
    }
    
    items.push_back(item);
    std::cout << item->getName() << " added to inventory." << std::endl;
    return true;
}

Item* Inventory::removeItem(int index) {
    if (index < 0 || index >= static_cast<int>(items.size())) {
        std::cout << "Invalid inventory index: " << index << std::endl;
        return nullptr;
    }
    
    Item* item = items[index];
    items.erase(items.begin() + index);
    return item;
}

bool Inventory::useItem(int index, Player& player) {
    if (index < 0 || index >= static_cast<int>(items.size())) {
        std::cout << "Invalid inventory index: " << index << std::endl;
        return false;
    }
    
    Item* item = items[index];
    item->use(player);
    
    // Remove the item after use
    delete item;
    items.erase(items.begin() + index);
    return true;
}

void Inventory::display() const {
    if (isEmpty()) {
        std::cout << "Your inventory is empty." << std::endl;
        return;
    }
    
    std::cout << "===== INVENTORY =====" << std::endl;
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << "[" << i + 1 << "] ";
        items[i]->display();
    }
    std::cout << "=====================" << std::endl;
}

bool Inventory::isEmpty() const {
    return items.empty();
}

bool Inventory::isFull() const {
    return static_cast<int>(items.size()) >= maxSize;
}

int Inventory::getItemCount() const {
    return static_cast<int>(items.size());
}

int Inventory::getMaxSize() const {
    return maxSize;
}

Item* Inventory::getItemAt(int index) const {
    if (index < 0 || index >= static_cast<int>(items.size())) {
        return nullptr;
    }
    return items[index];
}
