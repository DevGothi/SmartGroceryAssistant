#include "UserProfile.h"

#include <utility>

UserProfile::UserProfile()
    : UserProfile("", "", 0.0)
{
}

UserProfile::UserProfile(
    std::string userID,
    std::string name,
    double budgetLimit
)
    : userID(std::move(userID)),
      name(std::move(name)),
      budgetLimit(budgetLimit)
{
}

const std::string& UserProfile::getUserID() const
{
    return userID;
}

const std::string& UserProfile::getName() const
{
    return name;
}

double UserProfile::getBudgetLimit() const
{
    return budgetLimit;
}

void UserProfile::setName(const std::string& newName)
{
    name = newName;
}

void UserProfile::setBudgetLimit(double newBudgetLimit)
{
    if (newBudgetLimit >= 0)
    {
        budgetLimit = newBudgetLimit;
    }
}