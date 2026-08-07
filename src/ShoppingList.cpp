#include "ShoppingList.h"

#include <utility>

ShoppingList::ShoppingList()
    : ShoppingList("", false)
{
}

ShoppingList::ShoppingList(
    std::string listID,
    bool completed
)
    : listID(std::move(listID)),
      completed(completed)
{
}

const std::string& ShoppingList::getListID() const
{
    return listID;
}

bool ShoppingList::isCompleted() const
{
    return completed;
}

bool ShoppingList::addItem(
    const std::string& itemID,
    int quantity
)
{
    if (completed || itemID.empty() || quantity <= 0)
    {
        return false;
    }

    items[itemID] += quantity;
    return true;
}

bool ShoppingList::removeItem(const std::string& itemID)
{
    if (completed)
    {
        return false;
    }

    return items.erase(itemID) > 0;
}

bool ShoppingList::updateQuantity(
    const std::string& itemID,
    int quantity
)
{
    if (completed || quantity <= 0)
    {
        return false;
    }

    auto item = items.find(itemID);

    if (item == items.end())
    {
        return false;
    }

    item->second = quantity;
    return true;
}

double ShoppingList::computeTotalCost(
    const std::vector<GroceryItem>& groceryItems
) const
{
    double total = 0.0;

    for (const auto& listEntry : items)
    {
        const std::string& itemID = listEntry.first;
        const int listQuantity = listEntry.second;

        for (const auto& groceryItem : groceryItems)
        {
            if (groceryItem.getItemID() == itemID)
            {
                total += groceryItem.getPrice() * listQuantity;
                break;
            }
        }
    }

    return total;
}

void ShoppingList::markCompleted()
{
    completed = true;
}

const std::map<std::string, int>& ShoppingList::getItems() const
{
    return items;
}