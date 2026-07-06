#ifndef ITEMSEARCHDELETE_H
#define ITEMSEARCHDELETE_H

#include <vector>
#include <string>
#include "GroceryItem.h"

// Dev Gothi's part:
//   1. Grocery item SEARCH functionality
//   2. Grocery item REMOVE / DELETE functionality
//
// These are written as free functions that operate on the shared
// std::vector<GroceryItem> held by GroceryManager. When the team's real
// GroceryManager exists, these become member functions (GroceryManager::searchItems,
// GroceryManager::deleteItem) with almost no change.


// SEARCH
// - Case-insensitive.
// - An empty query returns ALL items (per the spec).
// - Matches against item NAME and CATEGORY.
// - Special characters are treated as plain text, so they can never crash it.
std::vector<GroceryItem> searchItems(const std::vector<GroceryItem>& items,
    const std::string& query);

// DELETE
// - Removes the item whose itemID matches (IDs are unique per the spec).
// - Returns true if an item was removed, false if nothing matched
//   (this is how the GUI knows to show "no item selected / not found").
bool deleteItem(std::vector<GroceryItem>& items, const std::string& itemID);

#endif // ITEMSEARCHDELETE_H