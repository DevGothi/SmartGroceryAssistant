#ifndef BUDGET_H
#define BUDGET_H

#include <string>
#include <vector>

#include "GroceryItem.h"

class Budget
{
private:
    std::string budgetID;
    double budgetLimit;

public:
    Budget();

    Budget(
        std::string budgetID,
        double budgetLimit
    );

    // Budget ID
    const std::string& getBudgetID() const;

    bool setBudgetID(
        const std::string& newBudgetID
    );

    static bool isValidBudgetID(
        const std::string& budgetID
    );

    // Budget limit
    bool setBudget(double budget);
    double getBudget() const;

    static bool isValidBudgetLimit(
        double budget
    );

    // Calculations
    double calculateTotalCost(
        const std::vector<GroceryItem>& items
    ) const;

    double getRemainingBudget(
        const std::vector<GroceryItem>& items
    ) const;

    double getUsagePercentage(
        const std::vector<GroceryItem>& items
    ) const;

    bool isNearBudget(
        const std::vector<GroceryItem>& items
    ) const;

    bool isOverBudget(
        const std::vector<GroceryItem>& items
    ) const;

    std::string getBudgetStatus(
        const std::vector<GroceryItem>& items
    ) const;
};

#endif