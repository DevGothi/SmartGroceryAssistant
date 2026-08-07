#ifndef GROCERYMANAGER_H
#define GROCERYMANAGER_H

#include <string>
#include <vector>

#include "GroceryItem.h"

class GroceryManager
{
private:
    std::vector<GroceryItem> items;

public:
    GroceryManager() = default;

    bool addItem(const GroceryItem& item);
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
};

#endif