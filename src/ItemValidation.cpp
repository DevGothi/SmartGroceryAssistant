#include "ItemValidation.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <utility>

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
                return static_cast<char>(std::tolower(character));
            }
        );

        return text;
    }

    std::optional<std::string> getValidCategory(
        const std::string& category
    )
    {
        static const std::array<std::string, 5> categories =
        {
            "Produce",
            "Dairy",
            "Meat",
            "Bakery",
            "Pantry"
        };

        const std::string loweredCategory =
            toLower(trim(category));

        for (const auto& allowedCategory : categories)
        {
            if (toLower(allowedCategory) == loweredCategory)
            {
                return allowedCategory;
            }
        }

        return std::nullopt;
    }

    std::optional<double> parsePrice(
        const std::string& priceText
    )
    {
        const std::string cleanedPrice = trim(priceText);

        if (cleanedPrice.empty())
        {
            return std::nullopt;
        }

        try
        {
            std::size_t charactersRead = 0;

            const double price = std::stod(
                cleanedPrice,
                &charactersRead
            );

            if (charactersRead != cleanedPrice.size())
            {
                return std::nullopt;
            }

            if (!std::isfinite(price))
            {
                return std::nullopt;
            }

            return price;
        }
        catch (const std::invalid_argument&)
        {
            return std::nullopt;
        }
        catch (const std::out_of_range&)
        {
            return std::nullopt;
        }
    }

    std::optional<int> parseQuantity(
        const std::string& quantityText
    )
    {
        const std::string cleanedQuantity =
            trim(quantityText);

        if (cleanedQuantity.empty())
        {
            return std::nullopt;
        }

        try
        {
            std::size_t charactersRead = 0;

            const long long quantity = std::stoll(
                cleanedQuantity,
                &charactersRead,
                10
            );

            if (charactersRead != cleanedQuantity.size())
            {
                return std::nullopt;
            }

            if (quantity < 1 || quantity > 10000)
            {
                return std::nullopt;
            }

            return static_cast<int>(quantity);
        }
        catch (const std::invalid_argument&)
        {
            return std::nullopt;
        }
        catch (const std::out_of_range&)
        {
            return std::nullopt;
        }
    }

    bool hasAlphabeticCharacter(const std::string& text)
    {
        return std::any_of(
            text.begin(),
            text.end(),
            [](unsigned char character)
            {
                return std::isalpha(character) != 0;
            }
        );
    }

    bool itemIDAlreadyExists(
        const std::vector<GroceryItem>& items,
        const std::string& candidateID,
        const std::optional<std::string>& ignoredItemID
    )
    {
        return std::any_of(
            items.begin(),
            items.end(),
            [&](const GroceryItem& item)
            {
                if (
                    ignoredItemID.has_value() &&
                    item.getItemID() == *ignoredItemID
                    )
                {
                    return false;
                }

                return item.getItemID() == candidateID;
            }
        );
    }

    bool identicalItemAlreadyExists(
        const std::vector<GroceryItem>& items,
        const ValidatedGroceryItemData& candidate,
        const std::optional<std::string>& ignoredItemID
    )
    {
        return std::any_of(
            items.begin(),
            items.end(),
            [&](const GroceryItem& item)
            {
                if (
                    ignoredItemID.has_value() &&
                    item.getItemID() == *ignoredItemID
                    )
                {
                    return false;
                }

                return
                    toLower(trim(item.getName())) ==
                    toLower(candidate.name) &&

                    toLower(item.getCategory()) ==
                    toLower(candidate.category) &&

                    std::fabs(
                        item.getPrice() - candidate.price
                    ) < 0.000001 &&

                    item.getQuantity() == candidate.quantity;
            }
        );
    }
}

bool ItemValidationResult::isValid() const noexcept
{
    return errors.empty() && data.has_value();
}

std::string ItemValidationResult::message() const
{
    if (isValid())
    {
        return "Valid grocery item.";
    }

    std::ostringstream output;

    for (std::size_t index = 0; index < errors.size(); ++index)
    {
        output << "- " << errors[index];

        if (index + 1 < errors.size())
        {
            output << '\n';
        }
    }

    return output.str();
}

ItemValidationResult validateGroceryItem(
    const GroceryItemInput& input,
    const std::vector<GroceryItem>& existingItems,
    const std::optional<std::string>& ignoredItemID
)
{
    ItemValidationResult result;
    ValidatedGroceryItemData cleanedData;

    cleanedData.itemID = trim(input.itemID);
    cleanedData.name = trim(input.name);

    // Required format: ITEM-0001
    static const std::regex itemIDPattern(
        R"(^ITEM-[0-9]{4}$)"
    );

    if (cleanedData.itemID.empty())
    {
        result.errors.emplace_back(
            "Item ID cannot be empty."
        );
    }
    else if (
        !std::regex_match(
            cleanedData.itemID,
            itemIDPattern
        )
        )
    {
        result.errors.emplace_back(
            "Item ID must use the format ITEM-0001."
        );
    }
    else if (
        itemIDAlreadyExists(
            existingItems,
            cleanedData.itemID,
            ignoredItemID
        )
        )
    {
        result.errors.emplace_back(
            "Item ID already exists."
        );
    }

    if (cleanedData.name.empty())
    {
        result.errors.emplace_back(
            "Item name cannot be empty."
        );
    }
    else
    {
        if (cleanedData.name.size() < 2)
        {
            result.errors.emplace_back(
                "Item name must contain at least 2 characters."
            );
        }

        if (cleanedData.name.size() > 100)
        {
            result.errors.emplace_back(
                "Item name cannot exceed 100 characters."
            );
        }

        if (!hasAlphabeticCharacter(cleanedData.name))
        {
            result.errors.emplace_back(
                "Item name must contain at least one letter."
            );
        }
    }

    const auto category =
        getValidCategory(input.category);

    if (!category.has_value())
    {
        result.errors.emplace_back(
            "Category must be Produce, Dairy, Meat, Bakery, or Pantry."
        );
    }
    else
    {
        cleanedData.category = *category;
    }

    const auto price = parsePrice(input.price);

    if (!price.has_value())
    {
        result.errors.emplace_back(
            "Price must be numeric."
        );
    }
    else if (*price <= 0.0)
    {
        result.errors.emplace_back(
            "Price must be greater than $0.00."
        );
    }
    else if (*price > 10000.0)
    {
        result.errors.emplace_back(
            "Price cannot exceed $10,000.00."
        );
    }
    else
    {
        cleanedData.price = *price;
    }

    const auto quantity =
        parseQuantity(input.quantity);

    if (!quantity.has_value())
    {
        result.errors.emplace_back(
            "Quantity must be a whole number from 1 to 10,000."
        );
    }
    else
    {
        cleanedData.quantity = *quantity;
    }

    const bool convertedValuesAvailable =
        category.has_value() &&
        price.has_value() &&
        quantity.has_value();

    if (
        convertedValuesAvailable &&
        !cleanedData.name.empty() &&
        identicalItemAlreadyExists(
            existingItems,
            cleanedData,
            ignoredItemID
        )
        )
    {
        result.errors.emplace_back(
            "An identical grocery item already exists."
        );
    }

    if (result.errors.empty())
    {
        result.data = std::move(cleanedData);
    }

    return result;
}