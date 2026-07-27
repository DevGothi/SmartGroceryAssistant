#ifndef SHOPPINGITEM_H
#define SHOPPINGITEM_H

#include <iosfwd>
#include <memory>
#include <string>

// Abstract base class for all shopping item types.
class ShoppingItem
{
public:
    ShoppingItem();

    ShoppingItem(
        std::string itemID,
        std::string name,
        std::string category,
        double price
    );

    ShoppingItem(const ShoppingItem&) = default;
    ShoppingItem(ShoppingItem&&) noexcept = default;

    ShoppingItem& operator=(const ShoppingItem&) = default;
    ShoppingItem& operator=(ShoppingItem&&) noexcept = default;

    virtual ~ShoppingItem() = default;

    const std::string& getItemID() const noexcept;
    const std::string& getName() const noexcept;
    const std::string& getCategory() const noexcept;
    double getPrice() const noexcept;

    void setItemID(std::string newItemID);
    void setName(std::string newName);
    void setCategory(std::string newCategory);
    void setPrice(double newPrice) noexcept;

    // Pure virtual functions make ShoppingItem abstract.
    virtual double getCost() const noexcept = 0;
    virtual void displayItem(std::ostream& output) const = 0;

    // Returns a smart-pointer copy of the derived object.
    virtual std::unique_ptr<ShoppingItem> clone() const = 0;

    bool operator==(const ShoppingItem& other) const noexcept;
    bool operator!=(const ShoppingItem& other) const noexcept;

    friend std::ostream& operator<<(
        std::ostream& output,
        const ShoppingItem& item
        );

protected:
    std::string itemID;
    std::string name;
    std::string category;
    double price;
};

#endif