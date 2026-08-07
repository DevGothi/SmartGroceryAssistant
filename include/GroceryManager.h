#ifndef GROCERYMANAGER_H
#define GROCERYMANAGER_H

#include <string>
#include <vector>

#include "GroceryItem.h"
#include "ShoppingList.h"
#include "Budget.h"
#include "UserProfile.h"
#include "PurchaseRecord.h"

class GroceryManager
{
private:
    std::vector<GroceryItem> items;
    std::vector<ShoppingList> shoppingLists;
    std::vector<Budget> budgets;
    std::vector<UserProfile> users;
    std::vector<PurchaseRecord> purchases;

public:
    GroceryManager() = default;

    // -------------------------------------------------
    // Grocery Items
    // -------------------------------------------------

    bool addItem(const GroceryItem& item);

    bool editItem(
        const std::string& originalItemID,
        const GroceryItem& updatedItem
    );

    bool deleteItem(const std::string& itemID);

    std::vector<GroceryItem> searchItems(
        const std::string& query
    ) const;

    std::vector<GroceryItem> filterByCategory(
        const std::string& category
    ) const;

    std::vector<GroceryItem>& getItems();
    const std::vector<GroceryItem>& getItems() const;

    bool itemIDExists(const std::string& itemID) const;

    // -------------------------------------------------
    // Shopping Lists
    // -------------------------------------------------

    bool addShoppingList(const ShoppingList& shoppingList);

    bool shoppingListIDExists(
        const std::string& listID
    ) const;

    std::vector<ShoppingList>& getShoppingLists();
    const std::vector<ShoppingList>& getShoppingLists() const;

    // -------------------------------------------------
    // Budgets
    // -------------------------------------------------

    bool addBudget(const Budget& budget);

    bool budgetIDExists(
        const std::string& budgetID
    ) const;

    std::vector<Budget>& getBudgets();
    const std::vector<Budget>& getBudgets() const;

    // -------------------------------------------------
    // Users
    // -------------------------------------------------

    bool addUser(const UserProfile& user);

    bool userIDExists(
        const std::string& userID
    ) const;

    std::vector<UserProfile>& getUsers();
    const std::vector<UserProfile>& getUsers() const;

    // -------------------------------------------------
    // Purchase Records
    // -------------------------------------------------

    bool addPurchase(
        const PurchaseRecord& purchase
    );

    bool purchaseIDExists(
        const std::string& purchaseID
    ) const;

    std::vector<PurchaseRecord>& getPurchases();
    const std::vector<PurchaseRecord>& getPurchases() const;

    void clear();
};

#endif