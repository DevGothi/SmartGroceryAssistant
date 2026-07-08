#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <string>
#include "GroceryItem.h"

class DataManager
{
private:
    std::vector<GroceryItem> items;

public:
    void addItem(const GroceryItem& item);
    std::vector<GroceryItem>& getItems();
    const std::vector<GroceryItem>& getItems() const;

    void loadSampleData();

    bool saveData(const std::string& filename) const;
    bool loadData(const std::string& filename);
};

#endif