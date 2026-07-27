#include "ItemCrud.h"
#include "ItemSearchDelete.h"

#include <algorithm>
#include <cctype>
#include <iterator>

namespace
{
    std::string trim(const std::string& text)
    {
        const auto first = std::find_if_not(
            text.begin(),
            text.end(),
            [](unsigned char character)
            {
                return std::isspace(character) != 0;
            }
        );

        if (first == text.end())
        {
            return "";
        }

        const auto last = std::find_if_not(
            text.rbegin(),
            text.rend(),
            [](unsigned char character)
            {
                return std::isspace(character) != 0;
            }
        ).base();

        return std::string(first, last);
    }

    ItemOperationResult validationFailed(
        const ItemValidationResult& validation
    )
    {
        return
        {
            false,
            validation.message(),
            validation.errors
        };
    }
}

std::optional<std::size_t> findItemIndexByID(
    const std::vector<GroceryItem>& items,
    const std::string& itemID
)
{
    const std::string cleanedID = trim(itemID);

    const auto iterator = std::find_if(
        items.begin(),
        items.end(),
        [&](const GroceryItem& item)
        {
            return item.getItemID() == cleanedID;
        }
    );

    if (iterator == items.end())
    {
        return std::nullopt;
    }

    return static_cast<std::size_t>(
        std::distance(items.begin(), iterator)
        );
}

ItemOperationResult addGroceryItem(
    std::vector<GroceryItem>& items,
    const GroceryItemInput& input
)
{
    const ItemValidationResult validation =
        validateGroceryItem(input, items);

    if (!validation.isValid())
    {
        return validationFailed(validation);
    }

    const auto& data = *validation.data;

    items.emplace_back(
        data.itemID,
        data.name,
        data.category,
        data.price,
        data.quantity
    );

    return
    {
        true,
        "Grocery item added successfully.",
        {}
    };
}

ItemOperationResult editGroceryItem(
    std::vector<GroceryItem>& items,
    const std::string& originalItemID,
    const GroceryItemInput& updatedInput
)
{
    const auto index =
        findItemIndexByID(items, originalItemID);

    if (!index.has_value())
    {
        return
        {
            false,
            "The grocery item could not be edited.",
            {"Original item ID does not exist."}
        };
    }

    const ItemValidationResult validation =
        validateGroceryItem(
            updatedInput,
            items,
            items[*index].getItemID()
        );

    if (!validation.isValid())
    {
        return validationFailed(validation);
    }

    const auto& data = *validation.data;

    items[*index] = GroceryItem(
        data.itemID,
        data.name,
        data.category,
        data.price,
        data.quantity
    );

    return
    {
        true,
        "Grocery item updated successfully.",
        {}
    };
}

ItemOperationResult removeGroceryItem(
    std::vector<GroceryItem>& items,
    const std::string& itemID
)
{
    const std::string cleanedID = trim(itemID);

    if (cleanedID.empty())
    {
        return
        {
            false,
            "No item was deleted.",
            {"Select an item before pressing Delete."}
        };
    }

    if (!deleteItem(items, cleanedID))
    {
        return
        {
            false,
            "No item was deleted.",
            {"The selected Item ID was not found."}
        };
    }

    return
    {
        true,
        "Grocery item deleted successfully.",
        {}
    };
}