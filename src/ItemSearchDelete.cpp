#include "ItemSearchDelete.h"

#include <algorithm>
#include <cctype>

namespace {

    // Helper: lower-case a copy of a string so comparisons are case-insensitive.
    // Using unsigned char with std::tolower avoids undefined behaviour on
    // negative values (which is what makes some special-character strings crash).
    std::string toLower(const std::string& text) {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    // Helper: does 'haystack' contain 'needle'? (both already lower-cased)
    bool contains(const std::string& haystack, const std::string& needle) {
        return haystack.find(needle) != std::string::npos;
    }

} // anonymous namespace

// 
// SEARCH
// 
std::vector<GroceryItem> searchItems(const std::vector<GroceryItem>& items,
    const std::string& query) {
    // Empty search returns everything (spec requirement).
    if (query.empty()) {
        return items;
    }

    const std::string q = toLower(query);
    std::vector<GroceryItem> matches;

    // Range-based for loop (modern C++ requirement).
    for (const auto& item : items) {
        const std::string name = toLower(item.getName());
        const std::string category = toLower(item.getCategory());

        // Match on name OR category. Special characters are just compared as
        // ordinary text here, so they can never crash the search.
        if (contains(name, q) || contains(category, q)) {
            matches.push_back(item);
        }
    }

    return matches;
}

// 
// DELETE
// 
bool deleteItem(std::vector<GroceryItem>& items, const std::string& itemID) {
    // Find the item with the matching unique ID.
    auto it = std::find_if(items.begin(), items.end(),
        [&itemID](const GroceryItem& item) {
            return item.getItemID() == itemID;
        });

    // Nothing matched -> tell the caller so the GUI can warn the user.
    if (it == items.end()) {
        return false;
    }

    items.erase(it);
    return true;
}