#include "GroceryItem.h"

#include <iomanip>
#include <ostream>
#include <utility>

GroceryItem::GroceryItem()
    : GroceryItem("", "", "", 0.0, 0)
{
}

GroceryItem::GroceryItem(
    std::string itemID,
    std::string name,
    std::string category,
    double price,
    int quantity
)
    : ShoppingItem(
        std::move(itemID),
        std::move(name),
        std::move(category),
        price
    ),
    quantity(quantity)
{
}

int GroceryItem::getQuantity() const noexcept
{
    return quantity;
}

void GroceryItem::setQuantity(int newQuantity) noexcept
{
    quantity = newQuantity;
}

double GroceryItem::getCost() const noexcept
{
    return price * static_cast<double>(quantity);
}

void GroceryItem::displayItem(std::ostream& output) const
{
    output
        << itemID
        << " | " << name
        << " | " << category
        << " | $" << std::fixed << std::setprecision(2) << price
        << " | Quantity: " << quantity
        << " | Cost: $" << getCost();
}

std::unique_ptr<ShoppingItem> GroceryItem::clone() const
{
    return std::make_unique<GroceryItem>(*this);
}