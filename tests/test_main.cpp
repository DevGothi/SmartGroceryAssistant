#include "GroceryItem.h"
#include "ItemCrud.h"
#include "ItemSearchDelete.h"
#include "ShoppingItem.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    int passed = 0;
    int failed = 0;

    void check(bool condition, const std::string& name)
    {
        if (condition)
        {
            ++passed;
            std::cout << "[PASS] " << name << '\n';
        }
        else
        {
            ++failed;
            std::cout << "[FAIL] " << name << '\n';
        }
    }
}

int main()
{
    std::vector<GroceryItem> items =
    {
        {"ITEM-0001", "Milk", "Dairy", 3.49, 2},
        {"ITEM-0002", "Apple", "Produce", 0.80, 12},
        {"ITEM-0003", "Bread", "Bakery", 2.25, 1}
    };

    // ADD TESTS
    auto result = addGroceryItem(
        items,
        {
            "ITEM-0004",
            "Chicken",
            "Meat",
            "9.50",
            "2"
        }
    );

    check(result.success, "Add valid grocery item");
    check(items.size() == 4, "Item count increased");

    result = addGroceryItem(
        items,
        {
            "ITEM-0004",
            "Cheese",
            "Dairy",
            "5.99",
            "1"
        }
    );

    check(!result.success, "Reject duplicate ID");

    result = addGroceryItem(
        items,
        {
            "4",
            "Cheese",
            "Dairy",
            "5.99",
            "1"
        }
    );

    check(!result.success, "Reject incorrect ID format");

    result = addGroceryItem(
        items,
        {
            "ITEM-0005",
            "123",
            "Dairy",
            "5.99",
            "1"
        }
    );

    check(!result.success, "Reject name without letters");

    result = addGroceryItem(
        items,
        {
            "ITEM-0005",
            "Cheese",
            "Invalid Category",
            "5.99",
            "1"
        }
    );

    check(!result.success, "Reject invalid category");

    result = addGroceryItem(
        items,
        {
            "ITEM-0005",
            "Cheese",
            "Dairy",
            "abc",
            "1"
        }
    );

    check(!result.success, "Reject non-numeric price");

    // EDIT TESTS
    result = editGroceryItem(
        items,
        "ITEM-0001",
        {
            "ITEM-0001",
            "Chocolate Milk",
            "Dairy",
            "4.49",
            "3"
        }
    );

    check(result.success, "Edit existing item");
    check(
        items[0].getName() == "Chocolate Milk",
        "Edited name stored"
    );

    // SEARCH TESTS
    check(
        searchItems(items, "milk").size() == 1,
        "Search by item name"
    );

    check(
        searchItems(items, "PRODUCE").size() == 1,
        "Case-insensitive category search"
    );

    check(
        searchItems(items, "").size() == items.size(),
        "Empty search returns all items"
    );

    check(
        searchItems(items, "@#$%").empty(),
        "Special-character search does not crash"
    );

    // DELETE TESTS
    const std::size_t previousSize = items.size();

    result = removeGroceryItem(
        items,
        "ITEM-0002"
    );

    check(result.success, "Delete existing item");

    check(
        items.size() == previousSize - 1,
        "Deleted item removed from vector"
    );

    result = removeGroceryItem(
        items,
        "ITEM-9999"
    );

    check(
        !result.success,
        "Missing item deletion handled"
    );

    // REQUIRED C++ FEATURES
    GroceryItem item(
        "ITEM-0100",
        "Rice",
        "Pantry",
        10.00,
        2
    );

    ShoppingItem& abstractReference = item;

    check(
        abstractReference.getCost() == 20.00,
        "Polymorphic getCost works"
    );

    std::unique_ptr<ShoppingItem> copy =
        item.clone();

    check(
        copy != nullptr &&
        copy->getItemID() == "ITEM-0100",
        "Smart pointer clone works"
    );

    GroceryItem sameID(
        "ITEM-0100",
        "Other Item",
        "Dairy",
        2.00,
        1
    );

    check(
        item == sameID,
        "Overloaded equality operator works"
    );

    std::ostringstream output;
    output << item;

    check(
        output.str().find("ITEM-0100") !=
        std::string::npos,
        "Friend stream operator works"
    );

    std::cout
        << "\nPassed: " << passed
        << "\nFailed: " << failed
        << '\n';

    return failed == 0 ? 0 : 1;
}