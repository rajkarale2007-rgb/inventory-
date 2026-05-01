#include "InventoryManager.hpp"
#include <iostream>
#include <limits>
#include <string>

void printMenu() {
    std::cout << "\n=== Inventory Management System ===\n";
    std::cout << "1. Add Item\n";
    std::cout << "2. Get Item\n";
    std::cout << "3. Update Item\n";
    std::cout << "4. Delete Item\n";
    std::cout << "5. List Items (Sorted by ID)\n";
    std::cout << "6. List Items (Sorted by Name)\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter choice: ";
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    InventoryManager manager;
    int choice;

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            clearInput();
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "Exiting...\n";
            break;
        }

        switch (choice) {
            case 1: {
                int id, qty;
                float price;
                std::string name;
                std::cout << "Enter ID (>0): ";
                std::cin >> id;
                std::cout << "Enter Name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter Quantity (>=0): ";
                std::cin >> qty;
                std::cout << "Enter Price (>=0): ";
                std::cin >> price;

                if (manager.addItem(id, name, qty, price)) {
                    std::cout << "Item added successfully.\n";
                } else {
                    std::cout << "Failed to add item. Check constraints (ID > 0, unique ID, non-empty name < 40 chars, Qty/Price >= 0).\n";
                }
                break;
            }
            case 2: {
                int id;
                std::cout << "Enter ID: ";
                std::cin >> id;
                Item item;
                if (manager.getItem(id, item)) {
                    std::cout << "Found: ID=" << item.id << ", Name=" << item.name 
                              << ", Qty=" << item.quantity << ", Price=" << item.price << "\n";
                } else {
                    std::cout << "Item not found.\n";
                }
                break;
            }
            case 3: {
                int id, qty;
                float price;
                std::string name;
                std::cout << "Enter ID to update: ";
                std::cin >> id;
                std::cout << "Enter New Name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter New Quantity: ";
                std::cin >> qty;
                std::cout << "Enter New Price: ";
                std::cin >> price;

                if (manager.updateItem(id, name, qty, price)) {
                    std::cout << "Item updated successfully.\n";
                } else {
                    std::cout << "Failed to update item.\n";
                }
                break;
            }
            case 4: {
                int id;
                std::cout << "Enter ID to delete: ";
                std::cin >> id;
                if (manager.deleteItem(id)) {
                    std::cout << "Item deleted successfully.\n";
                } else {
                    std::cout << "Failed to delete item. Not found.\n";
                }
                break;
            }
            case 5: {
                auto items = manager.getItemsSortedById();
                std::cout << "\n--- Items (Sorted by ID) ---\n";
                for (const auto& item : items) {
                    std::cout << "ID: " << item.id << " | Name: " << item.name 
                              << " | Qty: " << item.quantity << " | Price: " << item.price << "\n";
                }
                break;
            }
            case 6: {
                auto items = manager.getItemsSortedByName();
                std::cout << "\n--- Items (Sorted by Name) ---\n";
                for (const auto& item : items) {
                    std::cout << "ID: " << item.id << " | Name: " << item.name 
                              << " | Qty: " << item.quantity << " | Price: " << item.price << "\n";
                }
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
