#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include <vector>

#include "GroceryManager.h"

class DataManager
{
private:
    GroceryManager groceryManager;

public:
    DataManager() = default;

    // Access to all managed collections
    GroceryManager& getGroceryManager();
    const GroceryManager& getGroceryManager() const;

    // Kept for compatibility with the Qt GUI
    std::vector<GroceryItem>& getItems();
    const std::vector<GroceryItem>& getItems() const;

    void loadSampleData();

    // Qt-compatible functions
    bool saveData(const std::string& filename) const;
    bool loadData(const std::string& filename);

    // Functions used by console tests or other managers
    bool saveData(
        const GroceryManager& manager,
        const std::string& filename
    ) const;

    bool loadData(
        GroceryManager& manager,
        const std::string& filename
    ) const;
};

#endif