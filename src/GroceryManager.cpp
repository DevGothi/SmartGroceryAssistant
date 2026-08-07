#include "GroceryManager.h"

#include "ItemSearchDelete.h"

#include <algorithm>
#include <cctype>
#include <regex>

namespace
{
    std::string toLower(std::string text)
    {
        std::transform(
            text.begin(),
            text.end(),
            text.begin(),
            [](unsigned char character)
            {
                return static_cast<char>(
                    std::tolower(character)
                );
            }
        );

        return text;
    }

    bool isValidShoppingListID(
        const std::string& listID
    )
    {
        static const std::regex pattern(
            R"(^LIST-[0-9]{4}$)"
        );

        return std::regex_match(listID, pattern);
    }

    bool isValidUserID(
        const std::string& userID
    )
    {
        static const std::regex pattern(
            R"(^USR-[0-9]{4}$)"
        );

        return std::regex_match(userID, pattern);
    }
}

// -------------------------------------------------
// Grocery Items
// -------------------------------------------------

bool GroceryManager::addItem(
    const GroceryItem& item
)
{
    if (
        item.getItemID().empty() ||
        itemIDExists(item.getItemID())
    )
    {
        return false;
    }

    items.push_back(item);
    return true;
}

bool GroceryManager::deleteItem(
    const std::string& itemID
)
{
    return ::deleteItem(items, itemID);
}

std::vector<GroceryItem>
GroceryManager::searchItems(
    const std::string& query
) const
{
    return ::searchItems(items, query);
}

std::vector<GroceryItem>
GroceryManager::filterByCategory(
    const std::string& category
) const
{
    std::vector<GroceryItem> results;

    const std::string requestedCategory =
        toLower(category);

    for (const GroceryItem& item : items)
    {
        if (
            toLower(item.getCategory()) ==
            requestedCategory
        )
        {
            results.push_back(item);
        }
    }

    return results;
}

std::vector<GroceryItem>&
GroceryManager::getItems()
{
    return items;
}

const std::vector<GroceryItem>&
GroceryManager::getItems() const
{
    return items;
}

bool GroceryManager::itemIDExists(
    const std::string& itemID
) const
{
    return std::any_of(
        items.begin(),
        items.end(),
        [&](const GroceryItem& item)
        {
            return item.getItemID() == itemID;
        }
    );
}

// -------------------------------------------------
// Shopping Lists
// -------------------------------------------------

bool GroceryManager::addShoppingList(
    const ShoppingList& shoppingList
)
{
    if (
        !isValidShoppingListID(
            shoppingList.getListID()
        ) ||
        shoppingListIDExists(
            shoppingList.getListID()
        )
    )
    {
        return false;
    }

    shoppingLists.push_back(shoppingList);
    return true;
}

bool GroceryManager::shoppingListIDExists(
    const std::string& listID
) const
{
    return std::any_of(
        shoppingLists.begin(),
        shoppingLists.end(),
        [&](const ShoppingList& shoppingList)
        {
            return shoppingList.getListID() ==
                   listID;
        }
    );
}

std::vector<ShoppingList>&
GroceryManager::getShoppingLists()
{
    return shoppingLists;
}

const std::vector<ShoppingList>&
GroceryManager::getShoppingLists() const
{
    return shoppingLists;
}

// -------------------------------------------------
// Budgets
// -------------------------------------------------

bool GroceryManager::addBudget(
    const Budget& budget
)
{
    if (
        !Budget::isValidBudgetID(
            budget.getBudgetID()
        ) ||
        !Budget::isValidBudgetLimit(
            budget.getBudget()
        ) ||
        budgetIDExists(
            budget.getBudgetID()
        )
    )
    {
        return false;
    }

    budgets.push_back(budget);
    return true;
}

bool GroceryManager::budgetIDExists(
    const std::string& budgetID
) const
{
    return std::any_of(
        budgets.begin(),
        budgets.end(),
        [&](const Budget& budget)
        {
            return budget.getBudgetID() ==
                   budgetID;
        }
    );
}

std::vector<Budget>&
GroceryManager::getBudgets()
{
    return budgets;
}

const std::vector<Budget>&
GroceryManager::getBudgets() const
{
    return budgets;
}

// -------------------------------------------------
// Users
// -------------------------------------------------

bool GroceryManager::addUser(
    const UserProfile& user
)
{
    const bool validName =
        user.getName().size() >= 2 &&
        user.getName().size() <= 50;

    const bool validBudget =
        user.getBudgetLimit() > 0.0 &&
        user.getBudgetLimit() <= 1000000.0;

    if (
        !isValidUserID(user.getUserID()) ||
        !validName ||
        !validBudget ||
        userIDExists(user.getUserID())
    )
    {
        return false;
    }

    users.push_back(user);
    return true;
}

bool GroceryManager::userIDExists(
    const std::string& userID
) const
{
    return std::any_of(
        users.begin(),
        users.end(),
        [&](const UserProfile& user)
        {
            return user.getUserID() == userID;
        }
    );
}

std::vector<UserProfile>&
GroceryManager::getUsers()
{
    return users;
}

const std::vector<UserProfile>&
GroceryManager::getUsers() const
{
    return users;
}

// -------------------------------------------------
// Purchase Records
// -------------------------------------------------

bool GroceryManager::addPurchase(
    const PurchaseRecord& purchase
)
{
    const bool validPurchaseID =
        !purchase.getPurchaseID().empty();

    const bool validShoppingList =
        !purchase.getShoppingListID().empty() &&
        shoppingListIDExists(
            purchase.getShoppingListID()
        );

    const bool validDate =
        !purchase.getPurchaseDate().empty();

    const bool validAmount =
        purchase.getTotalSpent() >= 0.0;

    if (
        !validPurchaseID ||
        !validShoppingList ||
        !validDate ||
        !validAmount ||
        purchaseIDExists(
            purchase.getPurchaseID()
        )
    )
    {
        return false;
    }

    purchases.push_back(purchase);
    return true;
}

bool GroceryManager::purchaseIDExists(
    const std::string& purchaseID
) const
{
    return std::any_of(
        purchases.begin(),
        purchases.end(),
        [&](const PurchaseRecord& purchase)
        {
            return purchase.getPurchaseID() ==
                   purchaseID;
        }
    );
}

std::vector<PurchaseRecord>&
GroceryManager::getPurchases()
{
    return purchases;
}

const std::vector<PurchaseRecord>&
GroceryManager::getPurchases() const
{
    return purchases;
}

// -------------------------------------------------
// Utility
// -------------------------------------------------

void GroceryManager::clear()
{
    items.clear();
    shoppingLists.clear();
    budgets.clear();
    users.clear();
    purchases.clear();
}