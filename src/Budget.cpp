#include "Budget.h"

Budget::Budget()
{
    budgetLimit = 0;
}

void Budget::setBudget(double budget)
{
    budgetLimit = budget;
}

double Budget::getBudget() const
{
    return budgetLimit;
}

double Budget::calculateTotalCost(const std::vector<GroceryItem>& items) const
{
    double total = 0;

    for (const auto& item : items)
    {
        total += item.getPrice() * item.getQuantity();
    }

    return total;
}

bool Budget::isOverBudget(const std::vector<GroceryItem>& items) const
{
    return calculateTotalCost(items) > budgetLimit;
}

double Budget::getRemainingBudget(const std::vector<GroceryItem>& items) const
{
    return budgetLimit - calculateTotalCost(items);
}