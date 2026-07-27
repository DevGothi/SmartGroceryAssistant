#ifndef GROCERYITEM_H
#define GROCERYITEM_H

#include "ShoppingItem.h"

#include <memory>
#include <string>

class GroceryItem final : public ShoppingItem
{
public:
    // Constructor chaining.
    GroceryItem();

    GroceryItem(
        std::string itemID,
        std::string name,
        std::string category,
        double price,
        int quantity
    );

    GroceryItem(const GroceryItem&) = default;
    GroceryItem(GroceryItem&&) noexcept = default;

    GroceryItem& operator=(const GroceryItem&) = default;
    GroceryItem& operator=(GroceryItem&&) noexcept = default;

    ~GroceryItem() override = default;

    int getQuantity() const noexcept;
    void setQuantity(int newQuantity) noexcept;

    double getCost() const noexcept override;
    void displayItem(std::ostream& output) const override;

    std::unique_ptr<ShoppingItem> clone() const override;

private:
    int quantity;
};

#endif