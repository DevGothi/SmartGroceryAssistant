#include "Budget.h"

#include <regex>
#include <utility>

Budget::Budget()
    : budgetID(""),
      budgetLimit(0.0)
{
}

Budget::Budget(
    std::string budgetID,
    double budgetLimit
)
    : budgetID(""),
      budgetLimit(0.0)
{
    setBudgetID(budgetID);
    setBudget(budgetLimit);
}

const std::string& Budget::getBudgetID() const
{
    return budgetID;
}

bool Budget::setBudgetID(
    const std::string& newBudgetID
)
{
    if (!isValidBudgetID(newBudgetID))
    {
        return false;
    }

    budgetID = newBudgetID;
    return true;
}

bool Budget::isValidBudgetID(
    const std::string& budgetID
)
{
    static const std::regex pattern(
        R"(^BUD-[0-9]{4}$)"
    );

    return std::regex_match(
        budgetID,
        pattern
    );
}

bool Budget::isValidBudgetLimit(double budget)
{
    return budget > 0.0 &&
           budget <= 1000000.0;
}

bool Budget::setBudget(double budget)
{
    if (!isValidBudgetLimit(budget))
    {
        return false;
    }

    budgetLimit = budget;
    return true;
}

double Budget::getBudget() const
{
    return budgetLimit;
}

double Budget::calculateTotalCost(
    const std::vector<GroceryItem>& items
) const
{
    double total = 0.0;

    for (const GroceryItem& item : items)
    {
        total += item.getPrice() *
                 item.getQuantity();
    }

    return total;
}

double Budget::getRemainingBudget(
    const std::vector<GroceryItem>& items
) const
{
    return budgetLimit -
           calculateTotalCost(items);
}

double Budget::getUsagePercentage(
    const std::vector<GroceryItem>& items
) const
{
    if (budgetLimit <= 0.0)
    {
        return 0.0;
    }

    return (
        calculateTotalCost(items) /
        budgetLimit
    ) * 100.0;
}

bool Budget::isNearBudget(
    const std::vector<GroceryItem>& items
) const
{
    const double percentage =
        getUsagePercentage(items);

    return percentage >= 90.0 &&
           percentage <= 100.0;
}

bool Budget::isOverBudget(
    const std::vector<GroceryItem>& items
) const
{
    return calculateTotalCost(items) >
           budgetLimit;
}

std::string Budget::getBudgetStatus(
    const std::vector<GroceryItem>& items
) const
{
    if (isOverBudget(items))
    {
        return "Over budget";
    }

    if (isNearBudget(items))
    {
        return "Warning: approaching budget limit";
    }

    return "Within budget";
}