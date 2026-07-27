#ifndef ITEMVALIDATION_H
#define ITEMVALIDATION_H

#include "GroceryItem.h"

#include <optional>
#include <string>
#include <vector>

// Text received from the GUI.
//
// Price and quantity remain strings initially so incorrect input,
// such as "abc", can be handled without crashing.
struct GroceryItemInput
{
    std::string itemID;
    std::string name;
    std::string category;
    std::string price;
    std::string quantity;
};

struct ValidatedGroceryItemData
{
    std::string itemID;
    std::string name;
    std::string category;
    double price = 0.0;
    int quantity = 0;
};

struct ItemValidationResult
{
    std::vector<std::string> errors;
    std::optional<ValidatedGroceryItemData> data;

    bool isValid() const noexcept;
    std::string message() const;
};

ItemValidationResult validateGroceryItem(
    const GroceryItemInput& input,
    const std::vector<GroceryItem>& existingItems,
    const std::optional<std::string>& ignoredItemID = std::nullopt
);

#endif