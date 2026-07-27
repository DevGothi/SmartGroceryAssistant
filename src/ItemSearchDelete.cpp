#include "ItemSearchDelete.h"

#include <algorithm>
#include <cctype>

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

    bool contains(
        const std::string& completeText,
        const std::string& searchText
    )
    {
        return completeText.find(searchText) !=
            std::string::npos;
    }
}

std::vector<GroceryItem> searchItems(
    const std::vector<GroceryItem>& items,
    const std::string& query
)
{
    const std::string cleanedQuery = trim(query);

    // Empty search returns every item.
    if (cleanedQuery.empty())
    {
        return items;
    }

    const std::string loweredQuery =
        toLower(cleanedQuery);

    std::vector<GroceryItem> results;

    for (const auto& item : items)
    {
        const std::string loweredName =
            toLower(item.getName());

        const std::string loweredCategory =
            toLower(item.getCategory());

        if (
            contains(loweredName, loweredQuery) ||
            contains(loweredCategory, loweredQuery)
            )
        {
            results.push_back(item);
        }
    }

    return results;
}

bool deleteItem(
    std::vector<GroceryItem>& items,
    const std::string& itemID
)
{
    const auto iterator = std::find_if(
        items.begin(),
        items.end(),
        [&](const GroceryItem& item)
        {
            return item.getItemID() == itemID;
        }
    );

    if (iterator == items.end())
    {
        return false;
    }

    items.erase(iterator);
    return true;
}