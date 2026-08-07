#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>

class GroceryManager;

class DataManager
{
public:
    bool saveData(
        const GroceryManager& groceryManager,
        const std::string& filename
    ) const;

    bool loadData(
        GroceryManager& groceryManager,
        const std::string& filename
    ) const;
};

#endif