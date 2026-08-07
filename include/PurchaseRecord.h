#ifndef PURCHASERECORD_H
#define PURCHASERECORD_H

#include <string>

class PurchaseRecord
{
private:
    std::string purchaseID;
    std::string shoppingListID;
    std::string purchaseDate;
    double totalSpent;

public:
    PurchaseRecord();

    PurchaseRecord(
        std::string purchaseID,
        std::string shoppingListID,
        std::string purchaseDate,
        double totalSpent
    );

    const std::string& getPurchaseID() const;
    const std::string& getShoppingListID() const;
    const std::string& getPurchaseDate() const;
    double getTotalSpent() const;

    void setPurchaseDate(const std::string& newDate);
    void setTotalSpent(double newTotalSpent);

    void recordPurchase(double amount);
};

#endif