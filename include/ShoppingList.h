#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H

#include <map>
#include <string>
#include <vector>

#include "GroceryItem.h"

class ShoppingList
{
private:
    std::string listID;
    std::map<std::string, int> items;
    bool completed;

public:
    ShoppingList();

    ShoppingList(
        std::string listID,
        bool completed = false
    );

    const std::string& getListID() const;
    bool isCompleted() const;

    bool addItem(const std::string& itemID, int quantity);
    bool removeItem(const std::string& itemID);
    bool updateQuantity(const std::string& itemID, int quantity);

    double computeTotalCost(
        const std::vector<GroceryItem>& groceryItems
    ) const;

    void markCompleted();

    const std::map<std::string, int>& getItems() const;
};

#endif