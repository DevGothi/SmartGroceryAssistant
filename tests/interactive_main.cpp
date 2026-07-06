// ----------------------------------------------------------------------------
// Dev Gothi - INTERACTIVE demo for search / remove features.
// Instead of auto-printing results, this lets you (or the prof) type any
// search or delete live, proving the features work on real input.
//
// Compile (same as before, just swap the file):
//   cl /std:c++17 /EHsc /I include src\ItemSearchDelete.cpp tests\interactive_main.cpp /Fe:demo.exe
//   .\demo.exe
// ----------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include "GroceryItem.h"
#include "ItemSearchDelete.h"

static void printItems(const std::vector<GroceryItem>& items) {
    if (items.empty()) {
        std::cout << "   (no items found)\n";
        return;
    }
    std::cout << "   ID          | Name     | Category | Price  | Qty\n";
    std::cout << "   ------------------------------------------------\n";
    for (const auto& item : items) {
        std::cout << "   " << item.getItemID()
            << "   | " << item.getName()
            << " \t| " << item.getCategory()
            << " \t| $" << item.getPrice()
            << " \t| " << item.getQuantity() << "\n";
    }
}

int main() {
    // Sample data (test data is part of my testing lane).
    std::vector<GroceryItem> items = {
        {"ITEM-0001", "Milk",    "Dairy",   3.49, 2},
        {"ITEM-0002", "Cheddar", "Dairy",   6.99, 1},
        {"ITEM-0003", "Apple",   "Produce", 0.80, 12},
        {"ITEM-0004", "Chicken", "Meat",    9.50, 1},
        {"ITEM-0005", "Bread",   "Bakery",  2.25, 3},
    };

    std::cout << "==============================================\n";
    std::cout << " Smart Grocery Assistant - Search/Delete Demo\n";
    std::cout << " (Dev Gothi's part: search + remove features)\n";
    std::cout << "==============================================\n";

    while (true) {
        std::cout << "\nMenu:\n"
            << "  1 - Show all items\n"
            << "  2 - Search items (by name or category)\n"
            << "  3 - Delete an item (by ID)\n"
            << "  4 - Exit\n"
            << "Choose an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "\nAll items:\n";
            printItems(items);

        }
        else if (choice == "2") {
            std::cout << "Enter search text (empty shows all, "
                "case-insensitive, any characters are safe): ";
            std::string query;
            std::getline(std::cin, query);

            auto results = searchItems(items, query);
            std::cout << "\nResults for \"" << query << "\":\n";
            printItems(results);

        }
        else if (choice == "3") {
            std::cout << "Enter the Item ID to delete (e.g. ITEM-0002): ";
            std::string id;
            std::getline(std::cin, id);

            if (deleteItem(items, id)) {
                std::cout << "\nDeleted " << id << ". Remaining items:\n";
                printItems(items);
            }
            else {
                // Graceful handling: no such item (spec edge case).
                std::cout << "\nNo item with ID \"" << id
                    << "\" was found. Nothing was deleted.\n";
            }

        }
        else if (choice == "4") {
            std::cout << "Goodbye!\n";
            break;

        }
        else {
            // Invalid menu input handled gracefully too.
            std::cout << "Invalid option, please enter 1-4.\n";
        }
    }

    return 0;
}