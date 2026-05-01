#include "InventoryManager.hpp"
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>

void InventoryManager::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int InventoryManager::getValidId(const std::string& prompt) {
    int id;
    while (true) {
        std::cout << prompt;
        if (std::cin >> id && id > 0) {
            clearInputBuffer();
            return id;
        }
        std::cout << "Invalid input. ID must be a positive integer.\n";
        clearInputBuffer();
    }
}

std::string InventoryManager::getValidName(const std::string& prompt) {
    std::string name;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, name);
        if (!name.empty() && name.length() < 40) {
            return name;
        }
        std::cout << "Invalid input. Name cannot be empty and must be under 40 characters.\n";
    }
}

int InventoryManager::getValidQuantity(const std::string& prompt) {
    int qty;
    while (true) {
        std::cout << prompt;
        if (std::cin >> qty && qty >= 0) {
            clearInputBuffer();
            return qty;
        }
        std::cout << "Invalid input. Quantity must be >= 0.\n";
        clearInputBuffer();
    }
}

float InventoryManager::getValidPrice(const std::string& prompt) {
    float price;
    while (true) {
        std::cout << prompt;
        if (std::cin >> price && price >= 0.0f) {
            clearInputBuffer();
            return price;
        }
        std::cout << "Invalid input. Price must be >= 0.0.\n";
        clearInputBuffer();
    }
}

void InventoryManager::displayMenu() {
    std::cout << "\n--- Hybrid Inventory Manager ---\n"
              << "1. Add Item\n"
              << "2. View Item\n"
              << "3. Update Item\n"
              << "4. Delete Item\n"
              << "5. List Items\n"
              << "6. Exit\n"
              << "Select an option: ";
}

void InventoryManager::add() {
    Item item;
    item.id = getValidId("Enter Item ID: ");
    std::string name = getValidName("Enter Item Name: ");
    std::copy(name.begin(), name.end(), item.name);
    item.name[name.length()] = '\0';
    item.quantity = getValidQuantity("Enter Quantity: ");
    item.price = getValidPrice("Enter Price: ");
    item.is_deleted = 0;

    if (add_item(&item)) {
        std::cout << "Item added successfully.\n";
    } else {
        std::cout << "Failed to add item. Duplicate ID or file error.\n";
    }
}

void InventoryManager::view() {
    int id = getValidId("Enter Item ID to view: ");
    Item item;
    if (get_item(id, &item)) {
        std::cout << "ID: " << item.id << ", Name: " << item.name 
                  << ", Quantity: " << item.quantity << ", Price: $" 
                  << std::fixed << std::setprecision(2) << item.price << "\n";
    } else {
        std::cout << "Item not found.\n";
    }
}

void InventoryManager::update() {
    int id = getValidId("Enter Item ID to update: ");
    Item item;
    if (!get_item(id, &item)) {
        std::cout << "Item not found.\n";
        return;
    }

    std::cout << "Updating item (ID: " << id << ")\n";
    std::string name = getValidName("Enter New Name: ");
    std::copy(name.begin(), name.end(), item.name);
    item.name[name.length()] = '\0';
    item.quantity = getValidQuantity("Enter New Quantity: ");
    item.price = getValidPrice("Enter New Price: ");

    if (update_item(id, &item)) {
        std::cout << "Item updated successfully.\n";
    } else {
        std::cout << "Failed to update item.\n";
    }
}

void InventoryManager::remove() {
    int id = getValidId("Enter Item ID to delete: ");
    if (delete_item(id)) {
        std::cout << "Item deleted successfully.\n";
    } else {
        std::cout << "Item not found or already deleted.\n";
    }
}

void InventoryManager::list() {
    std::vector<Item> items(1000); // Max 1000 items for listing
    int count = list_items(items.data(), 1000);

    if (count == 0) {
        std::cout << "No items in inventory.\n";
        return;
    }

    items.resize(count);
    std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.id < b.id;
    });

    std::cout << "\nInventory List (" << count << " items):\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << std::left << std::setw(10) << "ID" 
              << std::setw(20) << "Name" 
              << std::setw(10) << "Quantity" 
              << "Price\n";
    std::cout << "--------------------------------------------------\n";
    
    for (const auto& item : items) {
        std::cout << std::left << std::setw(10) << item.id 
                  << std::setw(20) << item.name 
                  << std::setw(10) << item.quantity 
                  << "$" << std::fixed << std::setprecision(2) << item.price << "\n";
    }
    std::cout << "--------------------------------------------------\n";
}

void InventoryManager::run() {
    int choice;
    do {
        displayMenu();
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            choice = 0; // Trigger invalid choice
        }

        switch (choice) {
            case 1: add(); break;
            case 2: view(); break;
            case 3: update(); break;
            case 4: remove(); break;
            case 5: list(); break;
            case 6: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice. Please select 1-6.\n"; break;
        }
    } while (choice != 6);
}
