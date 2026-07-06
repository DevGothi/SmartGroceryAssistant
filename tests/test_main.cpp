// 
// Dev Gothi - Testing for search / remove features.
//
// This is a plain console test driver (no Qt needed) so you can RUN your
// features and SHOW they work during the Wednesday progress check, even before
// the GUI table is ready. It builds a few sample items, then tests search and
// delete against the spec's edge cases.
// 

#include <iostream>
#include <vector>
#include "GroceryItem.h"
#include "ItemSearchDelete.h"

static void printItems(const std::vector<GroceryItem>& items) {
    if (items.empty()) {
        std::cout << "   (no items)\n";
        return;
    }
    for (const auto& item : items) {
        std::cout << "   " << item.getItemID()
            << " | " << item.getName()
            << " | " << item.getCategory()
            << " | $" << item.getPrice()
            << " | qty " << item.getQuantity() << "\n";
    }
}

int main() {
    // Sample test data (this is Dev's testing data, part of the testing lane).
    std::vector<GroceryItem> items = {
        {"ITEM-0001", "Milk",     "Dairy",   3.49, 2},
        {"ITEM-0002", "Cheddar",  "Dairy",   6.99, 1},
        {"ITEM-0003", "Apple",    "Produce", 0.80, 12},
        {"ITEM-0004", "Chicken",  "Meat",    9.50, 1},
        {"ITEM-0005", "Bread",    "Bakery",  2.25, 3},
    };

    std::cout << "=== All items ===\n";
    printItems(items);

    //  SEARCH TESTS 
    std::cout << "\n=== Search 'milk' (by name) ===\n";
    printItems(searchItems(items, "milk"));

    std::cout << "\n=== Search 'DAIRY' (case-insensitive, by category) ===\n";
    printItems(searchItems(items, "DAIRY"));

    std::cout << "\n=== Search '' (empty -> returns ALL) ===\n";
    printItems(searchItems(items, ""));

    std::cout << "\n=== Search '@#$%' (special chars -> no crash, no match) ===\n";
    printItems(searchItems(items, "@#$%"));

    std::cout << "\n=== Search 'xyz' (no match) ===\n";
    printItems(searchItems(items, "xyz"));

    //  DELETE TESTS 
    std::cout << "\n=== Delete ITEM-0003 (exists) ===\n";
    std::cout << "   removed? " << (deleteItem(items, "ITEM-0003") ? "yes" : "no") << "\n";
    printItems(items);

    std::cout << "\n=== Delete ITEM-9999 (does not exist -> handled gracefully) ===\n";
    std::cout << "   removed? " << (deleteItem(items, "ITEM-9999") ? "yes" : "no") << "\n";
    printItems(items);

    std::cout << "\nAll tests ran without crashing.\n";
    return 0;
}