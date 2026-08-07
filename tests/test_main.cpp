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
#include <fstream>

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
    // COMPLETE GROCERY MANAGER COLLECTION TESTS

    GroceryManager completeManager;

    check(
        completeManager.addItem(
            GroceryItem("ITEM-2001", "Rice", "Pantry", 10.00, 2)
        ),
        "Complete manager adds grocery item"
    );

    ShoppingList savedList("LIST-2001");
    savedList.addItem("ITEM-2001", 2);

    check(
        completeManager.addShoppingList(savedList),
        "Complete manager adds shopping list"
    );

    check(
        completeManager.addBudget(
            Budget("BUD-2001", 50.00)
        ),
        "Complete manager adds budget"
    );

    check(
        completeManager.addUser(
            UserProfile("USR-2001", "Irvanah", 50.00)
        ),
        "Complete manager adds user"
    );

    check(
        completeManager.addPurchase(
            PurchaseRecord(
                "PURCHASE-2001",
                "LIST-2001",
                "2026-08-06",
                20.00
            )
        ),
        "Complete manager adds purchase"
    );

    DataManager jsonManager;

    check(
        jsonManager.saveData(
            completeManager,
            "complete_test_data.json"
        ),
        "JSON DataManager saves all collections"
    );

    GroceryManager loadedCompleteManager;

    check(
        jsonManager.loadData(
            loadedCompleteManager,
            "complete_test_data.json"
        ),
        "JSON DataManager loads all collections"
    );

    check(
        loadedCompleteManager.getItems().size() == 1 &&
        loadedCompleteManager.getShoppingLists().size() == 1 &&
        loadedCompleteManager.getBudgets().size() == 1 &&
        loadedCompleteManager.getUsers().size() == 1 &&
        loadedCompleteManager.getPurchases().size() == 1,
        "All JSON collections restored"
    );
    // -------------------------------------------------
// DATAMANAGER INVALID / CORRUPTED FILE TESTS
// -------------------------------------------------

DataManager safetyDataManager;

GroceryManager protectedManager;

protectedManager.addItem(
    GroceryItem(
        "ITEM-3001",
        "Protected Rice",
        "Pantry",
        5.00,
        2
    )
);

// 1. Missing file
check(
    !safetyDataManager.loadData(
        protectedManager,
        "this_file_does_not_exist.json"
    ),
    "Missing JSON file rejected"
);

check(
    protectedManager.getItems().size() == 1,
    "Missing file does not delete existing data"
);


// 2. Empty file
{
    std::ofstream file("empty_test.json");
}

check(
    !safetyDataManager.loadData(
        protectedManager,
        "empty_test.json"
    ),
    "Empty JSON file rejected"
);

check(
    protectedManager.getItems().size() == 1,
    "Empty file does not delete existing data"
);


// 3. Corrupted JSON
{
    std::ofstream file("corrupted_test.json");

    file << R"({
        "items": [
            this is definitely not valid JSON
        ]
    })";
}

check(
    !safetyDataManager.loadData(
        protectedManager,
        "corrupted_test.json"
    ),
    "Corrupted JSON rejected"
);

check(
    protectedManager.getItems().size() == 1,
    "Corrupted JSON does not delete existing data"
);


// 4. Missing required top-level sections
{
    std::ofstream file("missing_sections.json");

    file << R"({
        "items": []
    })";
}

check(
    !safetyDataManager.loadData(
        protectedManager,
        "missing_sections.json"
    ),
    "JSON with missing sections rejected"
);

check(
    protectedManager.getItems().size() == 1,
    "Missing sections do not delete existing data"
);


// 5. Duplicate item IDs
{
    std::ofstream file("duplicate_items.json");

    file << R"({
        "items": [
            {
                "itemID": "ITEM-4001",
                "name": "Milk",
                "category": "Dairy",
                "price": 3.50,
                "quantity": 1
            },
            {
                "itemID": "ITEM-4001",
                "name": "Bread",
                "category": "Bakery",
                "price": 2.50,
                "quantity": 1
            }
        ],
        "shoppingLists": [],
        "budgets": [],
        "users": [],
        "purchases": []
    })";
}

check(
    !safetyDataManager.loadData(
        protectedManager,
        "duplicate_items.json"
    ),
    "Duplicate item IDs rejected during load"
);

check(
    protectedManager.getItems().size() == 1,
    "Duplicate data does not replace existing data"
);


// 6. Shopping list references nonexistent item
{
    std::ofstream file("invalid_reference.json");

    file << R"({
        "items": [],
        "shoppingLists": [
            {
                "listID": "LIST-4001",
                "completed": false,
                "items": [
                    {
                        "itemID": "ITEM-9999",
                        "quantity": 1
                    }
                ]
            }
        ],
        "budgets": [],
        "users": [],
        "purchases": []
    })";
}

check(
    !safetyDataManager.loadData(
        protectedManager,
        "invalid_reference.json"
    ),
    "Shopping list with nonexistent item rejected"
);

check(
    protectedManager.getItems().size() == 1,
    "Invalid reference does not delete existing data"
);


// Final proof that our original protected data survived ALL failures
check(
    protectedManager.getItems()[0].getItemID()
        == "ITEM-3001",
    "Original data survives all failed loads"
);
    // -------------------------------------------------
// BUDGET VALIDATION AND WARNING TESTS
// -------------------------------------------------

check(
    Budget::isValidBudgetID("BUD-0001"),
    "Valid budget ID accepted"
);

check(
    !Budget::isValidBudgetID("0001"),
    "Budget ID without prefix rejected"
);

check(
    !Budget::isValidBudgetID("BUD-12"),
    "Incorrect budget ID format rejected"
);

check(
    Budget::isValidBudgetLimit(1000000.00),
    "Maximum budget limit accepted"
);

check(
    !Budget::isValidBudgetLimit(0.00),
    "Zero budget rejected"
);

check(
    !Budget::isValidBudgetLimit(-50.00),
    "Negative budget rejected"
);

check(
    !Budget::isValidBudgetLimit(1000000.01),
    "Budget above maximum rejected"
);


// Test 90% warning
std::vector<GroceryItem> warningItems = {
    GroceryItem(
        "ITEM-5001",
        "Test Item",
        "Pantry",
        90.00,
        1
    )
};

Budget warningBudget(
    "BUD-5001",
    100.00
);

check(
    warningBudget.isNearBudget(warningItems),
    "90 percent budget warning triggered"
);

check(
    !warningBudget.isOverBudget(warningItems),
    "90 percent usage is not over budget"
);

check(
    warningBudget.getBudgetStatus(warningItems)
        == "Warning: approaching budget limit",
    "Near-budget warning message correct"
);


// Test over budget
std::vector<GroceryItem> overBudgetItems = {
    GroceryItem(
        "ITEM-5002",
        "Expensive Item",
        "Pantry",
        110.00,
        1
    )
};

check(
    warningBudget.isOverBudget(overBudgetItems),
    "Over-budget condition detected"
);

check(
    warningBudget.getBudgetStatus(overBudgetItems)
        == "Over budget",
    "Over-budget message correct"
);


// -------------------------------------------------
// GROCERY MANAGER DUPLICATE COLLECTION TESTS
// -------------------------------------------------

GroceryManager validationManager;

ShoppingList validationList(
    "LIST-5001"
);

check(
    validationManager.addShoppingList(
        validationList
    ),
    "Valid shopping list accepted"
);

check(
    !validationManager.addShoppingList(
        validationList
    ),
    "Duplicate shopping list ID rejected"
);

ShoppingList badList(
    "5002"
);

check(
    !validationManager.addShoppingList(
        badList
    ),
    "Invalid shopping list ID rejected"
);


Budget validationBudget(
    "BUD-5001",
    100.00
);

check(
    validationManager.addBudget(
        validationBudget
    ),
    "Valid budget accepted by manager"
);

check(
    !validationManager.addBudget(
        validationBudget
    ),
    "Duplicate budget ID rejected"
);


UserProfile validationUser(
    "USR-5001",
    "Test User",
    100.00
);

check(
    validationManager.addUser(
        validationUser
    ),
    "Valid user accepted"
);

check(
    !validationManager.addUser(
        validationUser
    ),
    "Duplicate user ID rejected"
);

UserProfile invalidUser(
    "5002",
    "Invalid User",
    100.00
);

check(
    !validationManager.addUser(
        invalidUser
    ),
    "Invalid user ID rejected"
);


// -------------------------------------------------
// PURCHASE RECORD VALIDATION TESTS
// -------------------------------------------------

// Purchase must reference an existing shopping list.

PurchaseRecord validPurchase(
    "PURCHASE-5001",
    "LIST-5001",
    "2026-08-06",
    25.00
);

check(
    validationManager.addPurchase(
        validPurchase
    ),
    "Valid purchase accepted"
);

check(
    !validationManager.addPurchase(
        validPurchase
    ),
    "Duplicate purchase ID rejected"
);


PurchaseRecord missingListPurchase(
    "PURCHASE-5002",
    "LIST-9999",
    "2026-08-06",
    25.00
);

check(
    !validationManager.addPurchase(
        missingListPurchase
    ),
    "Purchase with nonexistent shopping list rejected"
);


PurchaseRecord emptyDatePurchase(
    "PURCHASE-5003",
    "LIST-5001",
    "",
    25.00
);

check(
    !validationManager.addPurchase(
        emptyDatePurchase
    ),
    "Purchase with empty date rejected"
);


PurchaseRecord negativePurchase(
    "PURCHASE-5004",
    "LIST-5001",
    "2026-08-06",
    -10.00
);

check(
    !validationManager.addPurchase(
        negativePurchase
    ),
    "Negative purchase amount rejected"
);

    // -------------------------------------------------
// JSON DUPLICATE COLLECTION ID TESTS
// -------------------------------------------------

// Duplicate shopping-list IDs
{
    std::ofstream file("duplicate_lists.json");

    file << R"({
        "items": [],
        "shoppingLists": [
            {
                "listID": "LIST-6001",
                "completed": false,
                "items": []
            },
            {
                "listID": "LIST-6001",
                "completed": false,
                "items": []
            }
        ],
        "budgets": [],
        "users": [],
        "purchases": []
    })";
}

check(
    !safetyDataManager.loadData(
        protectedManager,
        "duplicate_lists.json"
    ),
    "Duplicate shopping list IDs rejected during load"
);


// Duplicate budget IDs
{
    std::ofstream file("duplicate_budgets.json");

    file << R"({
        "items": [],
        "shoppingLists": [],
        "budgets": [
            {
                "budgetID": "BUD-6001",
                "limit": 100.0
            },
            {
                "budgetID": "BUD-6001",
                "limit": 200.0
            }
        ],
        "users": [],
        "purchases": []
    })";
}

check(
    !safetyDataManager.loadData(
        protectedManager,
        "duplicate_budgets.json"
    ),
    "Duplicate budget IDs rejected during load"
);


// Duplicate user IDs
{
    std::ofstream file("duplicate_users.json");

    file << R"({
        "items": [],
        "shoppingLists": [],
        "budgets": [],
        "users": [
            {
                "userID": "USR-6001",
                "name": "User One",
                "budgetLimit": 100.0
            },
            {
                "userID": "USR-6001",
                "name": "User Two",
                "budgetLimit": 200.0
            }
        ],
        "purchases": []
    })";
}

check(
    !safetyDataManager.loadData(
        protectedManager,
        "duplicate_users.json"
    ),
    "Duplicate user IDs rejected during load"
);


// Duplicate purchase IDs
{
    std::ofstream file("duplicate_purchases.json");

    file << R"({
        "items": [],
        "shoppingLists": [
            {
                "listID": "LIST-6002",
                "completed": false,
                "items": []
            }
        ],
        "budgets": [],
        "users": [],
        "purchases": [
            {
                "purchaseID": "PURCHASE-6001",
                "shoppingListID": "LIST-6002",
                "purchaseDate": "2026-08-06",
                "totalSpent": 10.0
            },
            {
                "purchaseID": "PURCHASE-6001",
                "shoppingListID": "LIST-6002",
                "purchaseDate": "2026-08-06",
                "totalSpent": 20.0
            }
        ]
    })";
}

check(
    !safetyDataManager.loadData(
        protectedManager,
        "duplicate_purchases.json"
    ),
    "Duplicate purchase IDs rejected during load"
);


// Make sure none of those failures destroyed existing data
check(
    protectedManager.getItems().size() == 1 &&
    protectedManager.getItems()[0].getItemID() == "ITEM-3001",
    "Existing data survives duplicate collection load failures"
);
    std::cout
        << "\nPassed: " << passed
        << "\nFailed: " << failed
        << '\n';

    return failed == 0 ? 0 : 1;
}