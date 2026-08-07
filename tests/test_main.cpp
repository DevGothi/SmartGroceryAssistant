#include "GroceryItem.h"
#include "ItemCrud.h"
#include "ItemSearchDelete.h"
#include "ShoppingItem.h"
#include "ShoppingList.h"
#include "UserProfile.h"
#include "PurchaseRecord.h"
#include "GroceryManager.h"
#include "Budget.h"
#include "DataManager.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

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
    // Existing tests above here...

    // SHOPPING LIST TESTS
    ShoppingList shoppingList("LIST-0001");

    check(
        shoppingList.getListID() == "LIST-0001",
        "Shopping list ID stored"
    );

    check(
        !shoppingList.isCompleted(),
        "New shopping list is not completed"
    );

    check(
        shoppingList.addItem("ITEM-0001", 2),
        "Add item to shopping list"
    );

    check(
        shoppingList.addItem("ITEM-0004", 1),
        "Add second item to shopping list"
    );

    check(
        shoppingList.getItems().size() == 2,
        "Shopping list contains two items"
    );

    const double shoppingListTotal =
        shoppingList.computeTotalCost(items);

    check(
        std::abs(shoppingListTotal - 18.48) < 0.001,
        "Shopping list total calculated correctly"
    );

    check(
        shoppingList.updateQuantity("ITEM-0001", 3),
        "Update shopping list item quantity"
    );

    check(
        shoppingList.getItems().at("ITEM-0001") == 3,
        "Updated quantity stored"
    );

    check(
        shoppingList.removeItem("ITEM-0004"),
        "Remove item from shopping list"
    );

    check(
        shoppingList.getItems().size() == 1,
        "Removed item no longer in shopping list"
    );

    shoppingList.markCompleted();

    check(
        shoppingList.isCompleted(),
        "Shopping list marked completed"
    );

    check(
        !shoppingList.addItem("ITEM-0003", 1),
        "Cannot add item to completed shopping list"
    );

    check(
        !shoppingList.updateQuantity("ITEM-0001", 5),
        "Cannot update completed shopping list"
    );

    check(
        !shoppingList.removeItem("ITEM-0001"),
        "Cannot remove item from completed shopping list"
    );
    // USER PROFILE TESTS

    UserProfile user(
        "USER-0001",
        "Irvanah",
        100.00
    );

    check(
        user.getUserID() == "USER-0001",
        "User profile ID stored"
    );

    check(
        user.getName() == "Irvanah",
        "User profile name stored"
    );

    check(
        std::abs(user.getBudgetLimit() - 100.00) < 0.001,
        "User budget limit stored"
    );

    user.setName("Irvanah Fossock");

    check(
        user.getName() == "Irvanah Fossock",
        "User profile name updated"
    );

    user.setBudgetLimit(150.00);

    check(
        std::abs(user.getBudgetLimit() - 150.00) < 0.001,
        "User budget limit updated"
    );

    user.setBudgetLimit(-50.00);

    check(
        std::abs(user.getBudgetLimit() - 150.00) < 0.001,
        "Negative budget rejected"
    );
    // PURCHASE RECORD TESTS

    PurchaseRecord purchase(
        "PURCHASE-0001",
        "LIST-0001",
        "2026-08-06",
        39.82
    );

    check(
        purchase.getPurchaseID() == "PURCHASE-0001",
        "Purchase ID stored"
    );

    check(
        purchase.getShoppingListID() == "LIST-0001",
        "Purchase shopping list ID stored"
    );

    check(
        purchase.getPurchaseDate() == "2026-08-06",
        "Purchase date stored"
    );

    check(
        std::abs(purchase.getTotalSpent() - 39.82) < 0.001,
        "Purchase total stored"
    );

    purchase.setPurchaseDate("2026-08-07");

    check(
        purchase.getPurchaseDate() == "2026-08-07",
        "Purchase date updated"
    );

    purchase.recordPurchase(75.50);

    check(
        std::abs(purchase.getTotalSpent() - 75.50) < 0.001,
        "Purchase amount recorded"
    );

    purchase.recordPurchase(-20.00);

    check(
        std::abs(purchase.getTotalSpent() - 75.50) < 0.001,
        "Negative purchase amount rejected"
    );
    // GROCERY MANAGER, BUDGET, AND DATA MANAGER TESTS

GroceryManager groceryManager;

check(
    groceryManager.addItem(
        GroceryItem("ITEM-1001", "Rice", "Pantry", 10.00, 2)
    ),
    "GroceryManager adds first item"
);

check(
    groceryManager.addItem(
        GroceryItem("ITEM-1002", "Milk", "Dairy", 4.00, 3)
    ),
    "GroceryManager adds second item"
);

check(
    !groceryManager.addItem(
        GroceryItem("ITEM-1001", "Duplicate", "Pantry", 1.00, 1)
    ),
    "GroceryManager rejects duplicate ID"
);

check(
    groceryManager.searchItems("milk").size() == 1,
    "GroceryManager searches items"
);

check(
    groceryManager.filterByCategory("Pantry").size() == 1,
    "GroceryManager filters by category"
);

Budget managerBudget;
managerBudget.setBudget(40.00);

check(
    std::abs(
        managerBudget.calculateTotalCost(
            groceryManager.getItems()
        ) - 32.00
    ) < 0.001,
    "Budget calculates GroceryManager total"
);

check(
    !managerBudget.isOverBudget(
        groceryManager.getItems()
    ),
    "Budget reports within limit"
);

managerBudget.setBudget(20.00);

check(
    managerBudget.isOverBudget(
        groceryManager.getItems()
    ),
    "Budget detects over-budget total"
);

DataManager dataManager;

check(
    dataManager.saveData(
        groceryManager,
        "test_grocery_data.txt"
    ),
    "DataManager saves GroceryManager items"
);

GroceryManager loadedManager;

check(
    dataManager.loadData(
        loadedManager,
        "test_grocery_data.txt"
    ),
    "DataManager loads GroceryManager items"
);

check(
    loadedManager.getItems().size() == 2,
    "Loaded item count is correct"
);

check(
    loadedManager.getItems()[0].getItemID() == "ITEM-1001",
    "Loaded first item is correct"
);

check(
    loadedManager.getItems()[1].getQuantity() == 3,
    "Loaded item quantity is correct"
);

check(
    loadedManager.deleteItem("ITEM-1002"),
    "GroceryManager deletes an item"
);

check(
    loadedManager.getItems().size() == 1,
    "Deleted item removed from GroceryManager"
);
    std::cout
        << "\nPassed: " << passed
        << "\nFailed: " << failed
        << '\n';

    return failed == 0 ? 0 : 1;
    std::cout
        << "\nPassed: " << passed
        << "\nFailed: " << failed
        << '\n';

    return failed == 0 ? 0 : 1;
}