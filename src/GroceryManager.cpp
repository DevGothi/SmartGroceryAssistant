#include "GroceryManager.h"

#include "ItemSearchDelete.h"

bool GroceryManager::addItem(const GroceryItem& item)
{
    if (itemIDExists(item.getItemID()))
    {
        return false;
    }

    items.push_back(item);
    return true;
}

bool GroceryManager::deleteItem(const std::string& itemID)
{
    return ::deleteItem(items, itemID);
}

std::vector<GroceryItem> GroceryManager::searchItems(
    const std::string& query
) const
{
    return ::searchItems(items, query);
}

std::vector<GroceryItem> GroceryManager::filterByCategory(
    const std::string& category
) const
{
    return ::searchItems(items, category);
}

std::vector<GroceryItem>& GroceryManager::getItems()
{
    return items;
}

const std::vector<GroceryItem>& GroceryManager::getItems() const
{
    return items;
}

bool GroceryManager::itemIDExists(const std::string& itemID) const
{
    for (const auto& item : items)
    {
        if (item.getItemID() == itemID)
        {
            return true;
        }
    }

    return false;
}