#include "PurchaseRecord.h"

#include <utility>

PurchaseRecord::PurchaseRecord()
    : PurchaseRecord("", "", "", 0.0)
{
}

PurchaseRecord::PurchaseRecord(
    std::string purchaseID,
    std::string shoppingListID,
    std::string purchaseDate,
    double totalSpent
)
    : purchaseID(std::move(purchaseID)),
      shoppingListID(std::move(shoppingListID)),
      purchaseDate(std::move(purchaseDate)),
      totalSpent(totalSpent)
{
}

const std::string& PurchaseRecord::getPurchaseID() const
{
    return purchaseID;
}

const std::string& PurchaseRecord::getShoppingListID() const
{
    return shoppingListID;
}

const std::string& PurchaseRecord::getPurchaseDate() const
{
    return purchaseDate;
}

double PurchaseRecord::getTotalSpent() const
{
    return totalSpent;
}

void PurchaseRecord::setPurchaseDate(const std::string& newDate)
{
    purchaseDate = newDate;
}

void PurchaseRecord::setTotalSpent(double newTotalSpent)
{
    if (newTotalSpent >= 0)
    {
        totalSpent = newTotalSpent;
    }
}

void PurchaseRecord::recordPurchase(double amount)
{
    if (amount >= 0)
    {
        totalSpent = amount;
    }
}