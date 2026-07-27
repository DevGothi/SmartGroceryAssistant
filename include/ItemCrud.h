#ifndef ITEMCRUD_H
#define ITEMCRUD_H

#include "ItemValidation.h"

#include <optional>
#include <string>
#include <vector>

struct ItemOperationResult
{
    bool success = false;
    std::string message;
    std::vector<std::string> errors;
};

std::optional<std::size_t> findItemIndexByID(
    const std::vector<GroceryItem>& items,
    const std::string& itemID
);

ItemOperationResult addGroceryItem(
    std::vector<GroceryItem>& items,
    const GroceryItemInput& input
);

ItemOperationResult editGroceryItem(
    std::vector<GroceryItem>& items,
    const std::string& originalItemID,
    const GroceryItemInput& updatedInput
);

ItemOperationResult removeGroceryItem(
    std::vector<GroceryItem>& items,
    const std::string& itemID
);

#endif