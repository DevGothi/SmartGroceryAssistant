#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <string>

class UserProfile
{
private:
    std::string userID;
    std::string name;
    double budgetLimit;

public:
    UserProfile();

    UserProfile(
        std::string userID,
        std::string name,
        double budgetLimit
    );

    const std::string& getUserID() const;
    const std::string& getName() const;
    double getBudgetLimit() const;

    void setName(const std::string& newName);
    void setBudgetLimit(double newBudgetLimit);
};

#endif