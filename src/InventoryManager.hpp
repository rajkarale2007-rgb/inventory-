#ifndef INVENTORY_MANAGER_HPP
#define INVENTORY_MANAGER_HPP

#include <vector>
#include <string>
#include "inventory.h"

class InventoryManager {
public:
    void run();

private:
    void displayMenu();
    void add();
    void view();
    void update();
    void remove();
    void list();

    // Input validation helpers
    int getValidId(const std::string& prompt);
    std::string getValidName(const std::string& prompt);
    int getValidQuantity(const std::string& prompt);
    float getValidPrice(const std::string& prompt);
    void clearInputBuffer();
};

#endif // INVENTORY_MANAGER_HPP
