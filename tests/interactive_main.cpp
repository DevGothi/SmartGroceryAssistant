// ----------------------------------------------------------------------------
// Dev Gothi + Irvanah - INTERACTIVE demo for search / remove / budget features.
// ----------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include "GroceryItem.h"
#include "ItemSearchDelete.h"
#include "Budget.h"
#include <iomanip>
#include "DataManager.h"

static void printItems(const std::vector<GroceryItem>& items) {
    if (items.empty()) {
        std::cout << "   (no items found)\n";
        return;
    }

    std::cout << std::left
              << std::setw(12) << "ID"
              << std::setw(12) << "Name"
              << std::setw(12) << "Category"
              << std::setw(10) << "Price"
              << std::setw(5) << "Qty"
              << '\n';

    std::cout << "--------------------------------------------------------\n";

    for (const auto& item : items) {
        std::cout << std::left
                  << std::setw(12) << item.getItemID()
                  << std::setw(12) << item.getName()
                  << std::setw(12) << item.getCategory()
                  << "$" << std::setw(9) << item.getPrice()
                  << std::setw(5) << item.getQuantity()
                  << '\n';
    }
}

int main() {

    DataManager manager;

    manager.loadSampleData();

    std::vector<GroceryItem>& items = manager.getItems();
    /*std::vector<GroceryItem> items = {
        {"ITEM-0001", "Milk",    "Dairy",   3.49, 2},
        {"ITEM-0002", "Cheddar", "Dairy",   6.99, 1},
        {"ITEM-0003", "Apple",   "Produce", 0.80, 12},
        {"ITEM-0004", "Chicken", "Meat",    9.50, 1},
        {"ITEM-0005", "Bread",   "Bakery",  2.25, 3},
    };*/

    Budget budget;
    budget.setBudget(50.00);

    std::cout << "==============================================\n";
    std::cout << " Smart Grocery Assistant Demo\n";
    std::cout << " Search/Delete + Budget Tracking\n";
    std::cout << "==============================================\n";

    while (true) {
        std::cout << "\nMenu:\n"
          << "  1 - Show all items\n"
          << "  2 - Search items (by name or category)\n"
          << "  3 - Delete an item (by ID)\n"
          << "  4 - Check budget\n"
          << "  5 - Save data\n"
          << "  6 - Load data\n"
          << "  7 - Exit\n"
          << "Choose an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "\nAll items:\n";
            printItems(items);
        }
        else if (choice == "2") {
            std::cout << "Enter search text: ";
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
                std::cout << "\nNo item with ID \"" << id
                    << "\" was found. Nothing was deleted.\n";
            }
        }
        else if (choice == "4") {
            std::cout << "\nBudget Summary:\n";
            std::cout << "Budget Limit: $" << budget.getBudget() << "\n";
            std::cout << "Total Cost: $" << budget.calculateTotalCost(items) << "\n";
            std::cout << "Remaining Budget: $" << budget.getRemainingBudget(items) << "\n";

            if (budget.isOverBudget(items)) {
                std::cout << "Warning: You are over budget!\n";
            }
            else {
                std::cout << "You are within budget.\n";
            }
        }
        else if (choice == "5") {
            if (manager.saveData("grocery.txt")) {
                std::cout << "\nData saved to grocery.txt\n";
            } else {
                std::cout << "\nError: Could not save data.\n";
            }
        }
        else if (choice == "6") {
            if (manager.loadData("grocery.txt")) {
                std::cout << "\nData loaded from grocery.txt\n";
                printItems(items);
            } else {
                std::cout << "\nError: Could not load data.\n";
            }
        }
        else if (choice == "7") {
            std::cout << "Goodbye!\n";
            break;
        }
        else {
            std::cout << "Invalid option, please enter 1-7.\n";
        }
    }

    return 0;
}