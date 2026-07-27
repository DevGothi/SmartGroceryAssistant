#include "ShoppingItem.h"

#include <ostream>
#include <utility>

ShoppingItem::ShoppingItem()
    : ShoppingItem("", "", "", 0.0)
{
}

ShoppingItem::ShoppingItem(
    std::string itemID,
    std::string name,
    std::string category,
    double price
)
    : itemID(std::move(itemID)),
    name(std::move(name)),
    category(std::move(category)),
    price(price)
{
}

const std::string& ShoppingItem::getItemID() const noexcept
{
    return itemID;
}

const std::string& ShoppingItem::getName() const noexcept
{
    return name;
}

const std::string& ShoppingItem::getCategory() const noexcept
{
    return category;
}

double ShoppingItem::getPrice() const noexcept
{
    return price;
}

void ShoppingItem::setItemID(std::string newItemID)
{
    itemID = std::move(newItemID);
}

void ShoppingItem::setName(std::string newName)
{
    name = std::move(newName);
}

void ShoppingItem::setCategory(std::string newCategory)
{
    category = std::move(newCategory);
}

void ShoppingItem::setPrice(double newPrice) noexcept
{
    price = newPrice;
}

bool ShoppingItem::operator==(const ShoppingItem& other) const noexcept
{
    return itemID == other.itemID;
}

bool ShoppingItem::operator!=(const ShoppingItem& other) const noexcept
{
    return !(*this == other);
}

std::ostream& operator<<(
    std::ostream& output,
    const ShoppingItem& item
    )
{
    item.displayItem(output);
    return output;
}