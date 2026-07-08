#ifndef BUDGET_H
#define BUDGET_H

#include <vector>
#include "GroceryItem.h"

class Budget
{
private:
    double budgetLimit;

public:
    Budget();

    void setBudget(double budget);
    double getBudget() const;

    double calculateTotalCost(const std::vector<GroceryItem>& items) const;
    bool isOverBudget(const std::vector<GroceryItem>& items) const;
    double getRemainingBudget(const std::vector<GroceryItem>& items) const;
};

#endif