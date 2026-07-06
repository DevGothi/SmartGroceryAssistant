#ifndef GROCERYITEM_H
#define GROCERYITEM_H

#include <string>

// 
// NOTE FOR THE TEAM:
// This is a MINIMAL version of the shared GroceryItem class so that Dev's
// search and delete features can compile and be tested on their own.
// The full version (abstract ShoppingItem base + GroceryItem derived, smart
// pointers, etc.) will be merged in later. Dev's functions only rely on the
// getters below, so they will keep working once the real class is dropped in.
// 

class GroceryItem {
public:
    GroceryItem(std::string id,
        std::string name,
        std::string category,
        double price,
        int quantity)
        : itemID(std::move(id)),
        name(std::move(name)),
        category(std::move(category)),
        price(price),
        quantity(quantity) {
    }

    // Getters (search/delete only need to read the item)
    const std::string& getItemID()   const { return itemID; }
    const std::string& getName()     const { return name; }
    const std::string& getCategory() const { return category; }
    double             getPrice()    const { return price; }
    int                getQuantity() const { return quantity; }

private:
    std::string itemID;
    std::string name;
    std::string category;
    double      price;
    int         quantity;
};

#endif // GROCERYITEM_H